//header file
#pragma once
#include "Mutator_base.h"

/**
 * NOEXCEPT_MACRO_EXPANSION_11
 */ 
class MutatorFrontendAction_NOEXCEPT_MACRO_EXPANSION_11 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(NOEXCEPT_MACRO_EXPANSION_11)

private:
    class MutatorASTConsumer_NOEXCEPT_MACRO_EXPANSION_11 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_NOEXCEPT_MACRO_EXPANSION_11(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite), macroInserted(false) {}
        virtual void run(const MatchFinder::MatchResult &Result);
    private:
        Rewriter &Rewrite;
        bool macroInserted;
    };
};

//source file
#include "../include/Mutator_NOEXCEPT_MACRO_EXPANSION_11.h"

// ========================================================================================================
#define MUTNOEXCEPT_MACRO_EXPANSION_11_OUTPUT 1

void MutatorFrontendAction_NOEXCEPT_MACRO_EXPANSION_11::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::CXXFunctionDecl>("funcDecl")) {
        // Filter nodes in header files
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                           FD->getLocation()))
            return;
        // Check if it's a function template
        auto funcTemplate = FD->getDescribedFunctionTemplate();
        if (!funcTemplate)
            return;
        // Get the first template parameter's name
        auto templateParams = funcTemplate->getTemplateParameters();
        if (templateParams.empty())
            return;
        auto firstParam = templateParams->getParam(0);
        if (!firstParam || !isa<clang::TemplateTypeParmDecl>(firstParam))
            return;
        StringRef TName = firstParam->getName();
        if (TName.empty())
            return;
        // Get the NoexceptExpr and its condition
        auto *NE = FD->getNoexceptExpr();
        if (!NE)
            return;
        auto *condExpr = NE->getExpr();
        if (!condExpr)
            return;
        SourceRange condRange = condExpr->getSourceRange();
        if (condRange.isInvalid())
            return;
        // Generate the replacement string
        std::string replacement = "NOEXCEPT_MACRO(std::declval<" + TName.str() + ">().nonconstexpr_method())";
        // Replace the original condition
        Rewrite.ReplaceText(condRange, replacement);
        // Insert the macro definition if not already inserted
        if (!macroInserted) {
            SourceLocation startLoc = Rewrite.getSourceMgr().getLocForStartOfFile(Rewrite.getSourceMgr().getMainFileID());
            Rewrite.InsertText(startLoc, "#define NOEXCEPT_MACRO(x) x\n", true, true);
            macroInserted = true;
        }
    }
}

void MutatorFrontendAction_NOEXCEPT_MACRO_EXPANSION_11::MutatorASTConsumer_NOEXCEPT_MACRO_EXPANSION_11::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define the ASTMatcher to identify function templates with noexcept specifiers
    DeclarationMatcher matcher = cxxFunctionDecl(isTemplate(), hasNoexceptSpecifier()).bind("funcDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}