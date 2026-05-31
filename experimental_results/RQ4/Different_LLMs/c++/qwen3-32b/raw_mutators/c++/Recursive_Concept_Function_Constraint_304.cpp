//header file
#pragma once
#include "Mutator_base.h"

/**
 * Recursive_Concept_Function_Constraint_304
 */ 
class MutatorFrontendAction_RecursiveConceptFunctionConstraint_304 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(RecursiveConceptFunctionConstraint_304)
private:
    class MutatorASTConsumer_RecursiveConceptFunctionConstraint_304 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_RecursiveConceptFunctionConstraint_304(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Recursive_Concept_Function_Constraint_304.h"

// ========================================================================================================
#define MUTRecursiveConceptFunctionConstraint_304_OUTPUT 1

void MutatorFrontendAction_RecursiveConceptFunctionConstraint_304::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("func")) {
        if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(FD->getLocation()))
            return;

        if (!FD->hasBody())
            return;

        if (FD->getTemplatedKind() != FunctionDecl::TK_NonTemplate)
            return;

        std::string conceptCode = "template<typename T>\nconcept RecursiveConstraint = requires(T t) { func(t); };\n";
        SourceLocation loc = FD->getBeginLoc();
        Rewrite.InsertText(loc, conceptCode, true, true);

        QualType returnType = FD->getReturnType();
        std::string returnTypeName = returnType.getAsString();
        std::string funcName = FD->getNameAsString();

        std::string newParams;
        for (size_t i = 0; i < FD->getNumParams(); ++i) {
            const ParmVarDecl *param = FD->getParamDecl(i);
            std::string paramType = (i == 0) ? "T" : param->getType().getAsString();
            std::string paramName = param->getNameAsString();
            if (!newParams.empty())
                newParams += ", ";
            newParams += paramType + " " + paramName;
        }

        std::string newFuncDecl = "template<typename T>\n" + returnTypeName + " " + funcName + "(" + newParams + ") requires RecursiveConstraint<T>";
        const Stmt *body = FD->getBody();
        std::string bodyStr = stringutils::rangetoStr(*Result.SourceManager, body->getSourceRange());
        std::string newFuncCode = newFuncDecl + " " + bodyStr;

        Rewrite.ReplaceText(FD->getSourceRange(), newFuncCode);
    }
}

void MutatorFrontendAction_RecursiveConceptFunctionConstraint_304::MutatorASTConsumer_RecursiveConceptFunctionConstraint_304::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl(
        unless(isTemplateInstantiation()),
        hasBody(stmt()),
        hasParameter(0, anything()),
        unless(hasRequiresClause())
    ).bind("func");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}