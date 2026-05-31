//source file
#include "../include/modify_template_deduction_guide_scope_12.h"

// ========================================================================================================
#define MUT12_OUTPUT 1

void MutatorFrontendAction_12::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TDG = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("TemplateDeductionGuide")) {
        if (!TDG || !Result.Context->getSourceManager().isWrittenInMainFile(TDG->getLocation()))
            return;

        const NamespaceDecl* enclosingNamespace = nullptr;
        for (const DeclContext* DC = TDG->getDeclContext(); DC; DC = DC->getParent()) {
            if (const auto* NS = dyn_cast<NamespaceDecl>(DC)) {
                enclosingNamespace = NS;
                break;
            }
        }

        if (enclosingNamespace && targetNamespace && enclosingNamespace != targetNamespace) {
            // Move the deduction guide inside the correct namespace
            SourceLocation startLoc = TDG->getBeginLoc();
            SourceLocation endLoc = TDG->getEndLoc().getLocWithOffset(1);
            std::string deductionGuideCode = Lexer::getSourceText(CharSourceRange::getTokenRange(startLoc, endLoc), Result.Context->getSourceManager(), LangOptions(), 0).str();
            Rewrite.RemoveText(CharSourceRange::getTokenRange(startLoc, endLoc));
            Rewrite.InsertTextAfter(targetNamespace->getRBraceLoc().getLocWithOffset(-1), "\n" + deductionGuideCode);
        } else if (enclosingNamespace && !targetNamespace) {
            // Move the deduction guide outside the namespace to introduce an error
            SourceLocation startLoc = TDG->getBeginLoc();
            SourceLocation endLoc = TDG->getEndLoc().getLocWithOffset(1);
            std::string deductionGuideCode = Lexer::getSourceText(CharSourceRange::getTokenRange(startLoc, endLoc), Result.Context->getSourceManager(), LangOptions(), 0).str();
            Rewrite.RemoveText(CharSourceRange::getTokenRange(startLoc, endLoc));
            Rewrite.InsertTextAfter(enclosingNamespace->getRBraceLoc().getLocWithOffset(1), "\n" + deductionGuideCode);
        }
    } else if (auto *NS = Result.Nodes.getNodeAs<clang::NamespaceDecl>("Namespace")) {
        if (!NS || !Result.Context->getSourceManager().isWrittenInMainFile(NS->getLocation()))
            return;
        targetNamespace = NS;
    }
}

void MutatorFrontendAction_12::MutatorASTConsumer_12::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher templateDeductionGuideMatcher = functionTemplateDecl().bind("TemplateDeductionGuide");
    DeclarationMatcher namespaceMatcher = namespaceDecl().bind("Namespace");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(templateDeductionGuideMatcher, &callback);
    matchFinder.addMatcher(namespaceMatcher, &callback);
    matchFinder.matchAST(Context);
}