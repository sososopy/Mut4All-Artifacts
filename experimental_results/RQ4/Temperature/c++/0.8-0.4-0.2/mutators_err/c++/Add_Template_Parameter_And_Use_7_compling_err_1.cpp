//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Template_Parameter_And_Use_7
 */ 
class MutatorFrontendAction_7 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(7)

private:
    class MutatorASTConsumer_7 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_7(Rewriter &R) : TheRewriter(R) {}
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
        std::string newTemplateParam;
    };
};

//source file
#include "../include/Mutator_.h"

// ========================================================================================================
#define MUT7_OUTPUT 1

void MutatorFrontendAction_7::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FuncTemplate = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FunctionTemplate")) {
        if (!FuncTemplate || !Result.Context->getSourceManager().isWrittenInMainFile(FuncTemplate->getLocation()))
            return;

        std::string templateParams = "typename U"; // Adding a new template parameter
        std::string newParamUsage = " + extra"; // Example usage of the new parameter

        if (auto *FuncDecl = FuncTemplate->getTemplatedDecl()) {
            if (FuncDecl->hasBody()) {
                std::string funcBody = stringutils::rangetoStr(*(Result.SourceManager), FuncDecl->getBody()->getSourceRange());
                std::string funcDecl = stringutils::rangetoStr(*(Result.SourceManager), FuncDecl->getSourceRange());

                size_t pos = funcDecl.find('(');
                if (pos != std::string::npos) {
                    funcDecl.insert(pos, ", U extra");
                }

                pos = funcBody.find_last_of('}');
                if (pos != std::string::npos) {
                    funcBody.insert(pos, "int result = value" + newParamUsage + ";");
                }

                std::string newTemplateDecl = "template<typename T, " + templateParams + ">\n" + funcDecl + funcBody;
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(FuncTemplate->getSourceRange()), newTemplateDecl);
            }
        }
    }
}

void MutatorFrontendAction_7::MutatorASTConsumer_7::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl().bind("FunctionTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}