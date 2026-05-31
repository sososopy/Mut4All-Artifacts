//header file
#pragma once
#include "Mutator_base.h"

/**
 * Wrap_Template_In_Inline_Namespace_415
 */ 
class MutatorFrontendAction_415 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(415)

private:
    class MutatorASTConsumer_415 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_415(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result);
    private:
        Rewriter &Rewrite;
    };
};

//source file
#include "../include/Wrap_Template_In_Inline_Namespace_415.h"

// ========================================================================================================
#define MUT415_OUTPUT 1

void MutatorFrontendAction_415::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::Decl>("template")) {
        if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(MT->getLocation()))
            return;
        
        if (!isa<clang::FunctionTemplateDecl>(MT) && !isa<clang::ClassTemplateDecl>(MT))
            return;
        
        clang::DeclContext *DC = MT->getDeclContext();
        while (DC && !isa<clang::NamespaceDecl>(DC)) {
            DC = DC->getParent();
        }
        if (DC) {
            clang::NamespaceDecl *ND = cast<clang::NamespaceDecl>(DC);
            if (ND->isInline()) {
                return;
            }
        }
        
        auto originalText = stringutils::rangetoStr(*Result.SourceManager, MT->getSourceRange());
        std::string newText = "inline namespace {\n" + originalText + "\n}";
        Rewrite.ReplaceText(MT->getSourceRange(), newText);
    }
}

void MutatorFrontendAction_415::MutatorASTConsumer_415::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = (functionTemplateDecl().bind("template") || classTemplateDecl().bind("template"));
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}