//header file
#pragma once
#include "Mutator_base.h"

/**
 * empty_variadic_template_pack_return_type_510
 */ 
class MutatorFrontendAction_510 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(510)

private:
    class MutatorASTConsumer_510 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_510(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_510.h"

// ========================================================================================================
#define MUT510_OUTPUT 1

void MutatorFrontendAction_510::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FuncTemplate")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       FD->getLocation()))
            return;

        if (FD->getTemplateParameters()->size() == 1) {
            const auto *TP = FD->getTemplateParameters()->getParam(0);
            if (const auto *TPP = llvm::dyn_cast<clang::TemplateTypeParmDecl>(TP)) {
                if (TPP->isParameterPack()) {
                    auto funcName = FD->getNameAsString();
                    std::string triggerCode = "void triggerCrash() {\n";
                    triggerCode += "    " + funcName + "<>::func<0>();\n";
                    triggerCode += "}\n";
                    Rewrite.InsertTextAfterToken(FD->getEndLoc(), "\n/*mut510*/" + triggerCode);
                }
            }
        }
    }
}
  
void MutatorFrontendAction_510::MutatorASTConsumer_510::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl(
        has(templateTypeParmDecl(hasType(templateTypeParmType())).bind("ParamPack")),
        has(functionDecl(returns(templateTypeParmType())))).bind("FuncTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}