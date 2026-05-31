//source file
#include "../include/alter_function_template_with_variadic_parameter_pack_509.h"

// ========================================================================================================
#define MUT509_OUTPUT 1

void MutatorFrontendAction_509::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FunctionTemplate")) {
        //Filter nodes in header files
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        if (FD->getTemplatedDecl()->hasBody()) {
            //Get the source code text of target node
            auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                       FD->getSourceRange());
            //Perform mutation on the source code text by applying string replacement
            size_t lambdaPos = declaration.find("[](auto...");
            if (lambdaPos != std::string::npos) {
                declaration.insert(lambdaPos + 3, "typename T, ");
                size_t bodyPos = declaration.find("{}", lambdaPos);
                if (bodyPos != std::string::npos) {
                    declaration.insert(bodyPos + 1, " T value; ");
                }
            }
            //Replace the original AST node with the mutated one
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), declaration);
        }
    }
}
  
void MutatorFrontendAction_509::MutatorASTConsumer_509::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionTemplateDecl(hasDescendant(lambdaExpr())).bind("FunctionTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}