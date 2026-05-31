//source file
#include "../include/Mutator_InsertInvalidTypeInMemberInitializer_360.h"

// ========================================================================================================
#define MUT360_OUTPUT 1

void MutatorFrontendAction_360::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *C = Result.Nodes.getNodeAs<clang::CXXConstructorDecl>("Constructor")) {
      //Filter nodes in header files
      if (!C || !Result.Context->getSourceManager().isWrittenInMainFile(
                     C->getLocation()))
        return;
      //Get the source code text of target node
      auto initList = C->init_begin();
      if (initList == C->init_end())
        return;
      auto memberVar = (*initList)->getMember();
      if (!memberVar)
        return;
      auto type = memberVar->getType().getAsString();
      //Perform mutation on the source code text by applying string replacement
      std::string invalidType = "InvalidType";
      std::string mutatedType = type + "/*mut360*/" + invalidType;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(memberVar->getSourceRange()), mutatedType);
    }
}
  
void MutatorFrontendAction_360::MutatorASTConsumer_360::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxConstructorDecl().bind("Constructor");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}