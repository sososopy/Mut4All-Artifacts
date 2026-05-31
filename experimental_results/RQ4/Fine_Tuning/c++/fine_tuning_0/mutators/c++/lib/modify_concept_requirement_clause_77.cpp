//source file
#include "../include/Modify_Concept_Requirement_Clause_77.h"

// ========================================================================================================
#define MUT77_OUTPUT 1

void MutatorFrontendAction_77::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::ConceptDecl>("Concepts")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto definition = stringutils::rangetoStr(*(Result.SourceManager),
                                                MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      llvm::outs() << definition << '\n';
      auto pos1 = definition.find("requires");
      auto pos2 = definition.find('{', pos1);
      auto pos3 = definition.find('}', pos2);
      auto req_clause = definition.substr(pos2 + 1, pos3 - pos2 - 1);
      llvm::outs() << req_clause << '\n';
      if (req_clause.find(',') != string::npos) {
        auto pos = req_clause.find(',');
        req_clause.erase(pos, 1);
      } else {
        req_clause += " 0";
      }
      llvm::outs() << req_clause << '\n';
      definition.replace(pos2 + 1, pos3 - pos2 - 1, req_clause);
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), definition);
    }
}
  
void MutatorFrontendAction_77::MutatorASTConsumer_77::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = conceptDecl().bind("Concepts");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}