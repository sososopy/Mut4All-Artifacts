//source file
#include "../include/Annotation_Token_Insertion_281.h"

// ========================================================================================================
#define MUT281_OUTPUT 1

void MutatorFrontendAction_281::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("VarDecls")) {
      //Filter nodes in header files
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                 DL->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      declaration = "/*mut281*/" + declaration;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(DL->getSourceRange()), declaration);
    }
}

void MutatorFrontendAction_281::MutatorASTConsumer_281::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = varDecl().bind("VarDecls");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}