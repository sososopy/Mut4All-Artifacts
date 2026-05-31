//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Templated_Method_With_Default_Template_Arg_458
 */ 
class MutatorFrontendAction_458 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(458)

private:
    class MutatorASTConsumer_458 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_458(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Insert_Templated_Method_With_Default_Template_Arg_458.h"

// ========================================================================================================
#define MUT458_OUTPUT 1

void MutatorFrontendAction_458::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CT = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("classTemplate")) {
        if (!CT || !Result.Context->getSourceManager().isWrittenInMainFile(CT->getLocation()))
            return;
        auto RD = CT->getTemplatedDecl();
        if (!RD)
            return;
        TemplateTemplateParmDecl *TTP = nullptr;
        for (auto param : CT->getTemplateParameters()) {
            TTP = dyn_cast<TemplateTemplateParmDecl>(param);
            if (TTP)
                break;
        }
        if (!TTP)
            return;
        std::string tTempName = TTP->getNameAsString();
        std::string methodCode = "    template <typename TArg>\n    void M(" + tTempName + "<TArg> = " + tTempName + "<TArg>()) { }\n";
        SourceLocation endLoc = RD->getEndLoc();
        Rewrite.InsertText(endLoc, methodCode, true, true);
    }
}

void MutatorFrontendAction_458::MutatorASTConsumer_458::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateDecl(
        hasTemplateParameterList(
            hasParameterPack(false),
            has(templateTemplateParm())
        )
    ).bind("classTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}