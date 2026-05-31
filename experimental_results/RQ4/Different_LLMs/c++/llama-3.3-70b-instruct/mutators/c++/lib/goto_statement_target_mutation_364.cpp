//source file
#include "../include/Goto_Statement_Target_Mutation_364.h"
#include "clang/AST/Stmt.h"
#include "clang/AST/Decl.h"

// ========================================================================================================
#define MUT364_OUTPUT 1

void MutatorFrontendAction_364::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *Goto = Result.Nodes.getNodeAs<clang::GotoStmt>("GotoStmt")) {
      //Filter nodes in header files
      if (!Goto || !Result.Context->getSourceManager().isWrittenInMainFile(
                     Goto->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto label = Goto->getLabel()->getNameAsString();
      //Perform mutation on the source code text by applying string replacement
      if (!labels.empty()) {
        auto newLabel = labels[0]; // getrandom::getRandomIndex(labels.size() - 1)
        std::string mutatedCode = "goto " + newLabel + ";";
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(Goto->getSourceRange()), mutatedCode);
      }
    } else if (auto *Label = Result.Nodes.getNodeAs<clang::LabelDecl>("LabelDecl")) {
      //Filter nodes in header files
      if (!Label || !Result.Context->getSourceManager().isWrittenInMainFile(
                     Label->getBeginLoc()))
        return;
      labels.push_back(Label->getNameAsString());
    }
}
  
void MutatorFrontendAction_364::MutatorASTConsumer_364::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher1 = stmt(clang::ast_matchers::gotoStmt().bind("GotoStmt"));
    auto matcher2 = decl(clang::ast_matchers::labelDecl().bind("LabelDecl"));
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher1, &callback);
    matchFinder.addMatcher(matcher2, &callback);
    matchFinder.matchAST(Context);
}