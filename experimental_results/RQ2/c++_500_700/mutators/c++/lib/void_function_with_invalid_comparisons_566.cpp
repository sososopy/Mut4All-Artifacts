//source file
#include "../include/Mutator_566.h"

// ========================================================================================================
#define MUT566_OUTPUT 1

void MutatorFrontendAction_566::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("VoidFunction")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;

      if (FD->hasBody() && FD->getReturnType()->isVoidType()) {
        //Get the source code text of target node
        auto functionBody = stringutils::rangetoStr(*(Result.SourceManager),
                                                    FD->getBody()->getSourceRange());
        //Perform mutation on the source code text by applying string replacement
        std::string mutation = R"(
          if (&&label_end - &&label_start < someVariable)
            )" + FD->getNameAsString() + R"(();
          label_start:
          goto *&&label_start;
          label_end:
          ;
        )";
        functionBody.insert(functionBody.find("{") + 1, mutation);
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getBody()->getSourceRange()), functionBody);
      }
    }
}
  
void MutatorFrontendAction_566::MutatorASTConsumer_566::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(isDefinition(), returns(voidType())).bind("VoidFunction");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}