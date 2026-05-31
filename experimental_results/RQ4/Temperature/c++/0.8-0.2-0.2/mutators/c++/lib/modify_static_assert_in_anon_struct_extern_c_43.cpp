//source file
#include "../include/modify_static_assert_in_anon_struct_extern_c_43.h"

// ========================================================================================================
#define MUT43_OUTPUT 1

void MutatorFrontendAction_43::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *AnonStruct = Result.Nodes.getNodeAs<clang::RecordDecl>("AnonStruct")) {
      //Filter nodes in header files
      if (!AnonStruct || !Result.Context->getSourceManager().isWrittenInMainFile(
                     AnonStruct->getLocation()))
        return;

      //Get the source code text of target node
      auto AnonStructCode = stringutils::rangetoStr(*(Result.SourceManager),
                                                    AnonStruct->getSourceRange());

      //Perform mutation on the source code text by applying string replacement
      std::string MutatedCode = "extern \"C\" {\n" + AnonStructCode + "\n}";
      
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(AnonStruct->getSourceRange()), MutatedCode);
    }
}
  
void MutatorFrontendAction_43::MutatorASTConsumer_43::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = recordDecl(
        isAnonymousStructOrUnion(),
        hasDescendant(staticAssertDecl())
    ).bind("AnonStruct");
    
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}