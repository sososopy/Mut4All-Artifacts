//header file
#pragma once
#include "Mutator_base.h"

/**
 * Recursive_Template_Instantiation_With_Function_Pointer_Nesting_26
 */ 
class MutatorFrontendAction_26 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(26)

private:
    class MutatorASTConsumer_26 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_26(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::ClassTemplateDecl *> matchedTemplates;
    };
};

//source file
#include "../include/Recursive_Template_Instantiation_With_Function_Pointer_Nesting_26.h"

// ========================================================================================================
#define MUT26_OUTPUT 1

void MutatorFrontendAction_26::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CTD = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("templateDecl")) {
        if (!CTD || !Result.Context->getSourceManager().isWrittenInMainFile(CTD->getLocation()))
            return;
        
        auto *RD = CTD->getTemplatedDecl();
        bool hasFuncPtrParam = false;
        for (const auto *Method : RD->methods()) {
            for (unsigned i = 0; i < Method->getNumParams(); ++i) {
                const auto *ParamType = Method->getParamDecl(i)->getType().getTypePtr();
                if (ParamType && ParamType->isFunctionPointerType()) {
                    hasFuncPtrParam = true;
                    break;
                }
            }
            if (hasFuncPtrParam) break;
        }
        if (!hasFuncPtrParam) return;
        
        // Insert new template parameter
        SourceRange TRange = CTD->getSourceRange();
        std::string TemplateParams = stringutils::rangetoStr(*Result.SourceManager, TRange);
        size_t Pos = TemplateParams.find('<');
        if (Pos != std::string::npos) {
            std::string NewParams = TemplateParams.substr(0, Pos + 1) + "int N = 5, ";
            NewParams += TemplateParams.substr(Pos + 1);
            Rewrite.ReplaceText(TRange, NewParams);
        }
        
        // Add base case specialization
        SourceLocation EndLoc = RD->getEndLoc();
        std::string BaseCase = "template<> struct A<0, void> { using Func = void; };\n";
        Rewrite.InsertText(EndLoc, BaseCase, true, true);
        
        // Modify method parameters
        for (const auto *Method : RD->methods()) {
            for (unsigned i = 0; i < Method->getNumParams(); ++i) {
                const auto *ParamDecl = Method->getParamDecl(i);
                const auto *ParamType = ParamDecl->getType().getTypePtr();
                if (ParamType && ParamType->isFunctionPointerType()) {
                    SourceRange TypeRange = ParamDecl->getTypeSourceInfo()->getTypeLoc().getSourceRange();
                    std::string NewType = "typename A<N-1, T>::Func (*)";
                    Rewrite.ReplaceText(TypeRange, NewType);
                }
            }
        }
        
        // Insert type alias
        for (const auto *Method : RD->methods()) {
            SourceLocation InsertLoc = Method->getBeginLoc();
            std::string AliasDecl = "using NestedFunc = typename A<N-1, T>::Func;\n";
            Rewrite.InsertText(InsertLoc, AliasDecl, true, true);
            break;
        }
    }
}

void MutatorFrontendAction_26::MutatorASTConsumer_26::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = classTemplateDecl().bind("templateDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}