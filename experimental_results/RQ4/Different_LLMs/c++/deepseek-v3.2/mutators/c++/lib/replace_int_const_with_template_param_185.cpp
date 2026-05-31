//source file
#include "../include/Mutator_Replace_Int_Const_With_Template_Param_185.h"

// ========================================================================================================
#define MUT185_OUTPUT 1

void MutatorFrontendAction_185::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *TP = Result.Nodes.getNodeAs<clang::TemplateTypeParmDecl>("TemplateParam")) {
      //Filter nodes in header files
      if (!TP || !Result.Context->getSourceManager().isWrittenInMainFile(
                     TP->getLocation()))
        return;
      //Record the node information to be used in the mutation process
      templateParams.push_back(TP);
    }
    else if (auto *BC = Result.Nodes.getNodeAs<clang::BinaryOperator>("BinaryComp")) {
      //Filter nodes in header files
      if (!BC || !Result.Context->getSourceManager().isWrittenInMainFile(
                     BC->getOperatorLoc()))
        return;
      if (!BC->isComparisonOp())
        return;
    }
}

void MutatorFrontendAction_185::MutatorASTConsumer_185::HandleTranslationUnit(ASTContext &Context) {
}