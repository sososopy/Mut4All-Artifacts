//source file
#include "../include/insert_unused_template_parameter_185.h"

// ========================================================================================================
#define MUT185_OUTPUT 1

void MutatorFrontendAction_185::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        if (visitedFunctions.find(FD) != visitedFunctions.end())
            return;

        visitedFunctions.insert(FD);

        if (FD->isTemplateInstantiation() || FD->getTemplatedKind() != clang::FunctionDecl::TK_NonTemplate)
            return;

        // Get the source code text of the target node
        auto functionText = stringutils::rangetoStr(*(Result.SourceManager), FD->getSourceRange());

        // Perform mutation on the source code text by applying string replacement
        std::string templateParam = "template<typename T /*mut185*/> ";
        functionText.insert(0, templateParam);

        // Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), functionText);
    }
}

void MutatorFrontendAction_185::MutatorASTConsumer_185::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define an ASTMatcher to identify the target AST node for mutation
    DeclarationMatcher matcher = functionDecl(unless(isTemplateInstantiation())).bind("Functions");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}