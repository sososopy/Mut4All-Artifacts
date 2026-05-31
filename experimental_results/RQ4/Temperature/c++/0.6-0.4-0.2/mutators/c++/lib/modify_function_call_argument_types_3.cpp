//source file
#include "../include/modify_function_call_argument_types_3.h"

// ========================================================================================================
#define MUT3_OUTPUT 1

void MutatorFrontendAction_3::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CE = Result.Nodes.getNodeAs<clang::CallExpr>("FunctionCall")) {
      //Filter nodes in header files
      if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CE->getExprLoc()))
        return;

      if (CE->getNumArgs() > 0) {
        //Get the source code text of target node
        auto arg = CE->getArg(0);
        auto argType = arg->getType().getAsString();
        std::string newType;
        
        //Determine a new type for mutation
        if (argType == "int") {
          newType = "double";
        } else if (argType == "double") {
          newType = "std::string";
        } else {
          newType = "int";
        }
        
        //Perform mutation on the source code text by applying string replacement
        std::string replacement = "/*mut3*/(" + newType + ")";
        Rewrite.InsertTextBefore(arg->getBeginLoc(), replacement);
      }
    }
}
  
void MutatorFrontendAction_3::MutatorASTConsumer_3::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = callExpr(callee(functionDecl(isTemplateInstantiation()))).bind("FunctionCall");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}