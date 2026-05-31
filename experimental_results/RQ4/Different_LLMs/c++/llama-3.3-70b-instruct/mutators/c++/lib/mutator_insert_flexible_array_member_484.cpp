//source file
#include "../include/Mutator_Insert_Flexible_Array_Member_484.h"

// ========================================================================================================
#define MUT484_OUTPUT 1

void MutatorFrontendAction_484::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::RecordDecl>("RecordDecls")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      std::string mutatedDeclaration = declaration;
      // Find the last field in the struct
      auto fields = MT->fields();
      if (!fields.empty()) {
        auto lastField = std::prev(fields.end());
        if (lastField->getType()->isArrayType()) {
          // Insert an initializer list for the flexible array member
          std::string initializerList = "{0}";
          mutatedDeclaration.insert(mutatedDeclaration.find("}"), initializerList);
        }
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), mutatedDeclaration);
    }
}
  
void MutatorFrontendAction_484::MutatorASTConsumer_484::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = recordDecl().bind("RecordDecls");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}