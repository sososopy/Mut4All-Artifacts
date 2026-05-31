//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Invalid_Template_Specialization_Boolean_319
 */ 
class MutatorFrontendAction_319 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(319)

private:
    class MutatorASTConsumer_319 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_319(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Invalid_Template_Specialization_Boolean_319.h"

// ========================================================================================================
#define MUT319_OUTPUT 1

void MutatorFrontendAction_319::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *TD = Result.Nodes.getNodeAs<clang::TemplateDecl>("templateDecl")) {
        if (!TD || !Result.Context->getSourceManager().isWrittenInMainFile(TD->getLocation()))
            return;

        TemplateParameterList *TPL = TD->getTemplateParameters();
        std::vector<std::string> typeParams;
        for (unsigned i = 0; i < TPL->size(); ++i) {
            TemplateParameter *Param = TPL->getParam(i);
            if (TypeTemplateParameterDecl *TTP = dyn_cast<TypeTemplateParameterDecl>(Param)) {
                std::string paramName = TTP->getNameAsString();
                if (!paramName.empty()) {
                    typeParams.push_back(paramName);
                    if (typeParams.size() >= 2) break;
                }
            }
        }
        if (typeParams.size() < 2) return;

        std::string firstParam = typeParams[0];
        std::string templateName = TD->getNameAsString();
        std::string partialCode = "template <typename " + firstParam + "> class " + templateName + "<" + firstParam + ", true>;\n";

        SourceLocation endLoc = TD->getEndLoc();
        Rewrite.InsertTextAfterToken(endLoc, partialCode);
    }
}

void MutatorFrontendAction_319::MutatorASTConsumer_319::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = templateDecl().bind("templateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}