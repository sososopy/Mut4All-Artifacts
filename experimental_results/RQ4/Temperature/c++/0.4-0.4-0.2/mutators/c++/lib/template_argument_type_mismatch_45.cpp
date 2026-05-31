//source file
#include "../include/template_argument_type_mismatch_45.h"

// ========================================================================================================
#define MUT45_OUTPUT 1

void MutatorFrontendAction_45::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CallExpr = Result.Nodes.getNodeAs<clang::CallExpr>("templateCall")) {
        //Filter nodes in header files
        if (!CallExpr || !Result.Context->getSourceManager().isWrittenInMainFile(
                CallExpr->getExprLoc()))
            return;

        const FunctionDecl *Callee = CallExpr->getDirectCallee();
        if (!Callee || !Callee->isTemplateInstantiation())
            return;

        const TemplateArgumentList *TemplateArgs = Callee->getTemplateSpecializationArgs();
        if (!TemplateArgs || TemplateArgs->size() == 0)
            return;

        // Perform mutation on the source code text by applying string replacement
        std::string replacementType = "double"; // Example replacement type
        std::string originalType = TemplateArgs->get(0).getAsType().getAsString();
        if (originalType == replacementType)
            return;

        // Replace the original AST node with the mutated one
        SourceLocation Start = CallExpr->getBeginLoc();
        SourceLocation End = CallExpr->getEndLoc();
        std::string callExprText = Lexer::getSourceText(CharSourceRange::getTokenRange(Start, End), 
                                                        Result.Context->getSourceManager(), 
                                                        Result.Context->getLangOpts()).str();
        std::string mutatedCallExprText = callExprText;
        size_t pos = mutatedCallExprText.find(originalType);
        if (pos != std::string::npos) {
            mutatedCallExprText.replace(pos, originalType.length(), replacementType);
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(Start, End), mutatedCallExprText);
        }
    }
}

void MutatorFrontendAction_45::MutatorASTConsumer_45::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = callExpr(callee(functionDecl(isTemplateInstantiation()))).bind("templateCall");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}