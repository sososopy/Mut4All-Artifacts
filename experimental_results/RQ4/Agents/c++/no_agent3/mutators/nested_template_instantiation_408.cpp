//header file
#pragma once
#include "Mutator_base.h"

/**
 * Nested_Template_Instantiation_408
 */ 
class MutatorFrontendAction_408 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(408)

private:
    class MutatorASTConsumer_408 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_408(Rewriter &R) : TheRewriter(R) {}
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
        std::set<std::string> existingTemplateNames;
    };
};

//source file
#include "../include/Nested_Template_Instantiation_408.h"

// ========================================================================================================
#define MUT408_OUTPUT 1

void MutatorFrontendAction_408::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FunctionTemplate")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        auto &SM = Rewrite.getSourceMgr();
        auto &LangOpts = Result.Context->getLangOpts();

        std::string originalSource = Lexer::getSourceText(CharSourceRange::getTokenRange(FD->getSourceRange()), SM, LangOpts).str();
        std::string newTemplateName = "Wrapper";
        
        if (existingTemplateNames.find(newTemplateName) == existingTemplateNames.end()) {
            std::string wrapperTemplate = "template <typename U> class Wrapper { /* Class implementation */ };\n";
            Rewrite.InsertTextBefore(FD->getBeginLoc(), wrapperTemplate);
            existingTemplateNames.insert(newTemplateName);
        }

        std::string modifiedSource = originalSource;
        size_t pos = modifiedSource.find("typename T");
        if (pos != std::string::npos) {
            modifiedSource.replace(pos, 10, "typename T, typename Wrapper<T>");
            pos = modifiedSource.find("T value");
            if (pos != std::string::npos) {
                modifiedSource.replace(pos, 7, "Wrapper<T> wrappedValue");
            }
        }

        Rewrite.ReplaceText(FD->getSourceRange(), modifiedSource);
    }
}

void MutatorFrontendAction_408::MutatorASTConsumer_408::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl().bind("FunctionTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}