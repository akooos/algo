#include <QScrollBar>

#include "ui_graph.h"
#include "graphalgorythm.h"
#include "util.h"
#include "graphicsview_node.h"
#include "graphicsview_edge.h"

GraphAlgorythm::GraphAlgorythm(GraphModel *gm):Algorythm(),scene(new GraphScene(gm,this)),ui(new Ui::GraphWidget()),gm(gm)
{

    ui->setupUi(wg);

    ui->tbAdd->setDefaultAction(ui->aAdd);
    ui->tbRemove->setDefaultAction(ui->aRemove);
    ui->tbSelect->setDefaultAction(ui->aNoAction);
    ui->tbAuto->setDefaultAction(ui->aAutoGen);
    ui->tbAddEdge->setDefaultAction(ui->aAddEdge);
    ui->tbRemoveEdge->setDefaultAction(ui->aRemoveEdge);

    bool check = QObject::connect(ui->aAdd,SIGNAL(triggered(bool)),this,SLOT(onActionTriggered(bool)));

    Q_ASSERT(check);

    check = QObject::connect(ui->aRemove,SIGNAL(triggered(bool)),this,SLOT(onActionTriggered(bool)));

    Q_ASSERT(check);

    check = QObject::connect(ui->aNoAction,SIGNAL(triggered(bool)),this,SLOT(onActionTriggered(bool)));

    Q_ASSERT(check);

    check = QObject::connect(ui->aAutoGen,SIGNAL(triggered(bool)),this,SLOT(onActionTriggered(bool)));

    Q_ASSERT(check);

    check = QObject::connect(ui->aAddEdge,SIGNAL(triggered(bool)),this,SLOT(onActionTriggered(bool)));

    Q_ASSERT(check);

    check = QObject::connect(ui->aRemoveEdge,SIGNAL(triggered(bool)),this,SLOT(onActionTriggered(bool)));

    Q_ASSERT(check);

    Q_UNUSED(check);

    ui->textBrowser->clear();

    ui->graphicsView->setBackgroundBrush(Qt::white);

    ui->graphicsView->setScene(scene);

    ui->tbAuto->setVisible(false);
    ui->leEdgeWeight->setVisible(false);
    ui->label->setVisible(false);

    scene->setSceneRect(0,0,ui->graphicsView->width(),ui->graphicsView->height());
}

GraphAlgorythm::~GraphAlgorythm()
{
  delete scene;
  delete ui;
  delete gm;
}

void GraphAlgorythm::printLine(const QString &line)
{
    QString txt = ui->textBrowser->toHtml();
    ui->textBrowser->setHtml( (txt + line).trimmed() );
    QScrollBar *sb = ui->textBrowser->verticalScrollBar();
    sb->setValue(sb->maximum());
    QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
}

void GraphAlgorythm::onActionTriggered(bool checked)
{
    QObject * o = QObject::sender();

    QAction *a = dynamic_cast<QAction*>(o);

     if ( a == ui->aAdd && checked  ){
         ui->aRemove->setChecked(false);
         ui->aAddEdge->setChecked(false);
         ui->aRemoveEdge->setChecked(false);
         scene->setMode(GraphScene::InsNode);
         return;
     }

     if ( a == ui->aRemove && checked  ){
         ui->aAdd->setChecked(false);
         ui->aAddEdge->setChecked(false);
         ui->aRemoveEdge->setChecked(false);
         scene->setMode(GraphScene::DelNode);
         return;
     }
     if ( a == ui->aAddEdge && checked  ){
         ui->aAdd->setChecked(false);
         ui->aRemove->setChecked(false);
         ui->aRemoveEdge->setChecked(false);
         scene->setMode(GraphScene::InsEdge);
         return;
     }
     if ( a == ui->aRemoveEdge && checked  ){
         ui->aAdd->setChecked(false);
         ui->aAddEdge->setChecked(false);
         ui->aRemove->setChecked(false);
         scene->setMode(GraphScene::DelEdge);
         return;
     }

     ui->aAdd->setChecked(false);
     ui->aRemove->setChecked(false);
     ui->aAddEdge->setChecked(false);
     ui->aRemoveEdge->setChecked(false);
     scene->setMode(GraphScene::NoAction);
}

void GraphAlgorythm::start()
{
    ui->aNoAction->trigger();

    QList<QGraphicsItem*> lgi = scene->selectedItems();

    GraphicsView_Node * node = 0;

    ui->textBrowser->clear();

    foreach(QGraphicsItem *gi , lgi){

        node = dynamic_cast<GraphicsView_Node*>(gi);

        if ( node )
            break;

       node = 0;
    }

    Graph<QString,QVariant> ::Node * startNode = 0;

    if ( node )
        startNode = gm->findNodeByValue(node->label());

    setState(AlgorythmStates::Running);
    runGraphAlgorythm(startNode);
    setState(AlgorythmStates::Stopped);

}

void GraphAlgorythm::stop()
{
    if ( m_state != AlgorythmStates::Running )
        return;

    setState(AlgorythmStates::Stopped);
}

GraphModel::GraphModel(GraphType graph_type): Graph<QString,QVariant>(graph_type){
    cntr = 0;
}

bool GraphModel::insertNode(QString &label, GraphicsView_Node *node){


    label = QString::fromStdString(Utils->numberToASCIINumber(cntr));
    cntr++;

    Graph<QString,QVariant>::Node *n = this->addNode(label);

    if (n){
        n->value("gnode",VPtr<GraphicsView_Node>::asQVariant(node));
        return true;
    }
    return false;
}

bool GraphModel::deleteNode(const QString &label){
    --cntr;
    return this->removeNode(label);
}

bool GraphModel::insertEdge(const QString &srcLabel, const QString &dstLabel, GraphicsView_Edge *edge){

    ITT( QString("Él hozzáadása: %1 - %2" ).arg(srcLabel).arg(dstLabel) )

            Graph<QString,QVariant>::Edge *masik_e = 0;
    Graph<QString,QVariant>::Edge *e = this->addEdge(srcLabel,dstLabel,&masik_e);


    QVariant vr = VPtr<GraphicsView_Edge>::asQVariant(edge);
    if (e){
        e->value("gedge",vr);
        if (masik_e){
            masik_e->value("gedge",vr);
        }
        return true;
    }
    return false;

}

bool GraphModel::deleteEdge(const QString &srcLabel, const QString &dstLabel){
    ITT( QString("Él törlése: %1 - %2" ).arg(srcLabel).arg(dstLabel) )
            return this->removeEdge(srcLabel,dstLabel);
}

void GraphModel::setNodeWeight(Graph::Node *n, QString key, QVariant value) {
    n->value(key,value);
}

bool GraphModel::setEdgeWeight(Graph::Edge *e, QString key, QVariant value) {
    e->value(key,value);
    return true;
}

bool GraphModel::setEdgeWeight(Graph::Node *src, Graph::Node *dest, QString key, QVariant value)
{
    Graph<QString,QVariant>::Edge *e = this->findEdge(src,dest);
    if ( !e )
        return false;

    return setEdgeWeight(e,key,value);
}

bool GraphModel::setEdgeWeight(const QString &srcLabel, const QString &dstLabel, QString key, QVariant value) {

    Graph<QString,QVariant>::Edge *e = this->findEdge(srcLabel,dstLabel);
    if ( !e )
        return false;

    ITT("SetWeight" << key << " = " << value.toString())
    return setEdgeWeight(e,key,value);


}

bool GraphModel::isDirected() const
{
    return  this->graphType() == AdjacencyList::Directed;
}
