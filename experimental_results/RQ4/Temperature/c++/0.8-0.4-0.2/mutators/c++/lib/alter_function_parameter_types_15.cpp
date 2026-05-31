//source file
#include "../include/Alter_Function_Parameter_Types_15.h"

// ========================================================================================================
#define MUT15_OUTPUT 1

void MutatorFrontendAction_15::Callback::run(const MatchFinder::MatchResult &Result) {
    // Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionWithPointerParam")) {
        // Filter nodes in header files
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        for (unsigned i = 0; i < FD->getNumParams(); ++i) {
            auto *param = FD->getParamDecl(i);
            if (param->getType()->isPointerType()) {
                // Get the source code text of target node
                auto paramType = param->getType().getAsString();
                auto paramName = param->getNameAsString();
                std::string newParamType = "int";
                // Perform mutation on the source code text by applying string replacement
                std::string newParamDecl = newParamType + " " + paramName;
                // Replace the original AST node with the mutated one
                Rewrite.ReplaceText(param->getSourceRange(), newParamDecl);
            }
        }
    }
}
  
void MutatorFrontendAction_15::MutatorASTConsumer_15::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(hasAnyParameter(hasType(pointerType()))).bind("FunctionWithPointerParam");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}