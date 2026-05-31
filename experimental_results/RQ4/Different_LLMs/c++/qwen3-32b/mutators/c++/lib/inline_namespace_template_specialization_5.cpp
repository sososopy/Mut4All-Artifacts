//source file
#include "../include/Mutator_Inline_Namespace_Template_Specialization_5.h"
#include "clang/AST/Decl.h"
#include "clang/AST/DeclTemplate.h"
#include "clang/AST/ASTContext.h"
#include "clang/Lex/Lexer.h"

// ========================================================================================================
#define MUT5_OUTPUT 1

void MutatorFrontendAction_5::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("templateDecl")) {
        // Filter nodes in header files
        if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(FTD->getLocation()))
            return;

        // Check if already in an inline namespace
        bool inInlineNamespace = false;
        for (const DeclContext *DC = FTD->getDeclContext(); DC; DC = DC->getParent()) {
            if (auto *NS = dyn_cast<clang::NamespaceDecl>(DC)) {
                if (NS->isInline()) {
                    inInlineNamespace = true;
                    break;
                }
            }
        }
        if (inInlineNamespace)
            return;

        // Get the original source text
        SourceManager &SM = *Result.SourceManager;
        SourceRange SR = FTD->getSourceRange();
        std::string originalText = clang::Lexer::getSourceText(clang::CharSourceRange::getTokenRange(SR), SM, Result.Context->getLangOpts()).str();

        // Create new text with inline namespace
        std::string newText = "inline namespace { " + originalText + " }";
        newText = "/*mut5*/" + newText;

        // Replace the original text
        Rewrite.ReplaceText(clang::CharSourceRange::getTokenRange(SR), newText);
    }
}

void MutatorFrontendAction_5::MutatorASTConsumer_5::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define ASTMatcher to identify function templates not in inline namespaces
    DeclarationMatcher matcher = functionTemplateDecl().bind("templateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}