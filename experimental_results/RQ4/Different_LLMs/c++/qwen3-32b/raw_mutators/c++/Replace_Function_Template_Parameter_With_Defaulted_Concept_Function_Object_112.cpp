//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Function_Template_Parameter_With_Defaulted_Concept_Function_Object_112
 */ 
class MutatorFrontendAction_112 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(112)

private:
    class MutatorASTConsumer_112 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_112(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Function_Template_Parameter_With_Defaulted_Concept_Function_Object_112.h"

// ========================================================================================================
#define MUT112_OUTPUT 1

void MutatorFrontendAction_112::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("func")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        clang::ASTContext *Context = Result.Context;
        clang::SourceManager &SM = Context->getSourceManager();
        clang::Rewriter &Rewriter = this->Rewrite;

        bool found = false;
        for (unsigned i = 0; i < FD->getNumParams() && !found; ++i) {
            clang::ParmVarDecl *Param = FD->getParamDecl(i);
            clang::QualType ParamType = Param->getType();
            if (clang::PointerType *PtrType = ParamType->getAs<clang::PointerType>()) {
                clang::QualType FuncType = PtrType->getPointeeType();
                if (clang::FunctionProtoType *FuncProtoType = FuncType->getAs<clang::FunctionProtoType>()) {
                    if (FuncProtoType->getNumParams() == 1) {
                        clang::QualType ReturnType = FuncProtoType->getReturnType();
                        if (ReturnType->isBooleanType()) {
                            clang::QualType TType = FuncProtoType->getParamType(0);
                            clang::PrintingPolicy Policy = Context->getPrintingPolicy();
                            std::string TStr = TType.getAsString(Policy);

                            std::string TemplateParam = "template<std::predicate<" + TStr + "> F = decltype([](" + TStr + " x) { return x; })>";
                            std::string NewParam = "F f = F{}";

                            clang::SourceLocation ReturnTyStart = FD->getReturnTypeSourceRange().getBegin();
                            Rewriter.InsertText(ReturnTyStart, TemplateParam + " ", 0, true);

                            Rewriter.ReplaceText(Param->getSourceRange(), NewParam);
                            found = true;
                        }
                    }
                }
            }
        }
    }
}

void MutatorFrontendAction_112::MutatorASTConsumer_112::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(
        unless(isTemplateInstantiation()),
        hasAnyParameter(hasType(pointer(qualType(functionType(returns(booleanType()))))))
    ).bind("func");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}