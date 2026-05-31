//source file
#include "../include/insert_unused_template_instantiation_134.h"
#include "clang/ASTMatchers/ASTMatchers.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include <iterator>

// ========================================================================================================
#define MUT134_OUTPUT 1

void MutatorFrontendAction_134::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("Templates")) {
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(TD->getLocation()))
            return;

        if (TD->isThisDeclarationADefinition()) {
            std::string templateName = TD->getNameAsString();
            templateNames.insert(templateName);
        }
    } else if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        if (!templateNames.empty()) {
            std::string randomTemplate = *std::next(templateNames.begin(), getrandom::getRandomIndex(templateNames.size()));
            std::string instantiation = "\n/*mut134*/ " + randomTemplate + "<int> unusedInstance;\n";
            Rewrite.InsertTextBefore(FD->getBody()->getBeginLoc(), instantiation);
        }
    }
}

void MutatorFrontendAction_134::MutatorASTConsumer_134::HandleTranslationUnit(ASTContext &Context) {
    using namespace clang::ast_matchers;
    MatchFinder matchFinder;
    DeclarationMatcher templateMatcher = classTemplateDecl().bind("Templates");
    DeclarationMatcher functionMatcher = functionDecl(hasBody(stmt())).bind("Functions");
    
    Callback callback(TheRewriter);
    matchFinder.addMatcher(templateMatcher, &callback);
    matchFinder.addMatcher(functionMatcher, &callback);
    matchFinder.matchAST(Context);
}