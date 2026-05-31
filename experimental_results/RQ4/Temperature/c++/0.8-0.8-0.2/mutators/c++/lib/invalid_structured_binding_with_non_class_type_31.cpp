//source file
#include "../include/invalid_structured_binding_with_non_class_type_31.h"

// ========================================================================================================
#define MUT31_OUTPUT 1

void MutatorFrontendAction_31::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("func")) {
        //Filter nodes in header files
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
            return;

        if (!FD->isTemplateInstantiation())
            return;

        // Perform mutation on the source code text by applying string replacement
        if (auto *BD = Result.Nodes.getNodeAs<clang::BindingDecl>("binding")) {
            if (BD->getType()->isFundamentalType()) {
                std::string newCode = "int nonClassType = 10;\nconst auto& [a, b] = nonClassType;";
                SourceLocation startLoc = BD->getBeginLoc();
                SourceLocation endLoc = BD->getEndLoc();
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(startLoc, endLoc), newCode);
            }
        }
    }
}
  
void MutatorFrontendAction_31::MutatorASTConsumer_31::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = functionDecl(isTemplateInstantiation()).bind("func");
    auto bindingMatcher = bindingDecl().bind("binding");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(bindingMatcher, &callback);
    matchFinder.matchAST(Context);
}