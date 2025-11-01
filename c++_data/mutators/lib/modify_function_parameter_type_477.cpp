//source file
#include "../include/modify_function_parameter_type_477.h"

// ========================================================================================================
#define MUT477_OUTPUT 1

void MutatorFrontendAction_477::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionDecl")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      
      if (FD->getNumParams() > 0) {
        firstParam = FD->getParamDecl(0);
        auto paramType = firstParam->getType();
        std::string newTypeStr;
        
        // Modify the type of the first parameter
        if (paramType->isPointerType()) {
          newTypeStr = paramType.getAsString() + "*";
        } else {
          newTypeStr = paramType.getAsString() + "*";
        }

        //Get the source code text of target node
        auto paramRange = firstParam->getSourceRange();
        std::string paramText = stringutils::rangetoStr(*(Result.SourceManager), paramRange);
        
        //Perform mutation on the source code text by applying string replacement
        std::string newParamText = newTypeStr + " " + firstParam->getNameAsString();
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(paramRange), newParamText);
      }
    }
}

void MutatorFrontendAction_477::MutatorASTConsumer_477::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(hasParameter(0, parmVarDecl().bind("FirstParam"))).bind("FunctionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}