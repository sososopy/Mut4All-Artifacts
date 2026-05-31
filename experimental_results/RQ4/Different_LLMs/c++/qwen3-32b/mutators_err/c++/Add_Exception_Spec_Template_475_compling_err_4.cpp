//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Exception_Spec_Template_475
 */ 
class MutatorFrontendAction_475 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(475)

private:
    class MutatorASTConsumer_475 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_475(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
    };
};

//source file
#include "../include/Add_Exception_Spec_Template_475.h"

// ========================================================================================================
#define MUT475_OUTPUT 1

void MutatorFrontendAction_475::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("funcTemplate")) {
        if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(FTD->getLocation()))
            return;

        FunctionDecl *FD = FTD->getTemplatedDecl();
        if (!FD)
            return;

        SourceManager &SM = *Result.SourceManager;
        std::string decl = stringutils::rangetoStr(SM, FD->getSourceRange());
        if (decl.empty())
            return;

        TemplateParameterList *TPL = FTD->getTemplateParameters();
        std::vector<std::string> paramNames;
        for (auto *Param : *TPL) {
            if (auto *TTP = dyn_cast<TemplateTypeParmDecl>(Param)) {
                paramNames.push_back(TTP->getNameAsString());
                if (paramNames.size() >= 2)
                    break;
            }
        }

        if (paramNames.size() < 2)
            return;

        std::string t = paramNames[0];
        std::string u = paramNames[1];

        std::string noexceptExpr = "noexcept(noexcept(std::declval<" + t + ">().call(std::declval<" + u + ">())))";

        size_t pos = decl.rfind(')');
        if (pos == std::string::npos)
            return;

        std::string newDecl = decl.substr(0, pos + 1) + " " + noexceptExpr + decl.substr(pos + 1);

        Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), newDecl);
    } else if (auto *MDecl = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("methodTemplate")) {
        if (!MDecl || !Result.Context->getSourceManager().isWrittenInMainFile(MDecl->getLocation()))
            return;

        FunctionTemplateDecl *FTD = MDecl->getDescribedFunctionTemplate();
        if (!FTD)
            return;

        FunctionDecl *FD = FTD->getTemplatedDecl();
        SourceManager &SM = *Result.SourceManager;
        std::string decl = stringutils::rangetoStr(SM, FD->getSourceRange());
        if (decl.empty())
            return;

        TemplateParameterList *TPL = FTD->getTemplateParameters();
        std::vector<std::string> paramNames;
        for (auto *Param : *TPL) {
            if (auto *TTP = dyn_cast<TemplateTypeParmDecl>(Param)) {
                paramNames.push_back(TTP->getNameAsString());
                if (paramNames.size() >= 2)
                    break;
            }
        }

        if (paramNames.size() < 2)
            return;

        std::string t = paramNames[0];
        std::string u = paramNames[1];

        std::string noexceptExpr = "noexcept(noexcept(std::declval<" + t + ">().call(std::declval<" + u + ">())))";

        size_t pos = decl.rfind(')');
        if (pos == std::string::npos)
            return;

        std::string newDecl = decl.substr(0, pos + 1) + " " + noexceptExpr + decl.substr(pos + 1);

        Rewrite.ReplaceText(CharSourceRange::getTokenRange(FD->getSourceRange()), newDecl);
    }
}

void MutatorFrontendAction_475::MutatorASTConsumer_475::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = 
        functionTemplateDecl().bind("funcTemplate") || 
        cxxMethodDecl(ofClass(isTemplated())).bind("methodTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}