//header file
#pragma once
#include "Mutator_base.h"

/**
 * ConceptLambdaTemplateMismatch_61
 */ 
class MutatorFrontendAction_61 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(61)

private:
    class MutatorASTConsumer_61 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_61(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::TemplateParameter *> functionTemplates;
    };
};

//source file
#include "../include/ConceptLambdaTemplateMismatch_61.h"

// ========================================================================================================
#define MUT61_OUTPUT 1

void MutatorFrontendAction_61::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("functionTemplate")) {
        if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(FT->getLocation()))
            return;

        const auto *FD = FT->getTemplatedDecl();
        if (!FD || !FD->getBody())
            return;

        const auto *Lambda = Result.Nodes.getNodeAs<clang::LambdaExpr>("lambda");
        if (!Lambda)
            return;

        const auto *CallOp = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("lambdaCallOp");
        if (!CallOp)
            return;

        const auto *Param = Result.Nodes.getNodeAs<clang::ParmVarDecl>("lambdaParam");
        if (!Param)
            return;

        const auto *ConstrainedType = Result.Nodes.getNodeAs<clang::Type>("constrainedType");
        if (!ConstrainedType)
            return;

        const auto *ConstraintExpr = ConstrainedType->getAs<clang::ConstrainedType>()->getConstraintExpr();
        if (!ConstraintExpr)
            return;

        const auto *TemplateParams = FT->getTemplateParameters();
        if (!TemplateParams || TemplateParams->size() < 2)
            return;

        std::vector<const clang::TemplateParameter *> outerParams;
        for (unsigned i = 0; i < TemplateParams->size(); ++i) {
            outerParams.push_back(cast<clang::TemplateParameter>(TemplateParams->getParam(i)));
        }

        const auto *ConceptSpec = dyn_cast<clang::ConceptSpecializationExpr>(ConstraintExpr);
        if (!ConceptSpec)
            return;

        const auto *Args = ConceptSpec->getTemplateArgumentsAsWritten();
        if (!Args || Args->size() < 1)
            return;

        const auto &Arg = Args->get(0);
        if (Arg.getKind() != clang::TemplateArgument::Type)
            return;

        const auto *TypeArg = Arg.getAsType();
        const auto *TypeParm = TypeArg->getTypePtr()->getAs<clang::TemplateTypeParmType>();
        if (!TypeParm)
            return;

        const auto *ParmDecl = TypeParm->getDecl();
        if (!ParmDecl)
            return;

        bool found = false;
        for (const auto *OuterParm : outerParams) {
            if (ParmDecl == OuterParm) {
                found = true;
                break;
            }
        }
        if (!found)
            return;

        int index = 0;
        for (size_t i = 0; i < outerParams.size(); ++i) {
            if (outerParams[i] == ParmDecl) {
                index = i;
                break;
            }
        }

        int newIndex = (index + 1) % outerParams.size();
        while (newIndex == index) {
            newIndex = (newIndex + 1) % outerParams.size();
        }

        const auto *NewParmDecl = dyn_cast<clang::TemplateTypeParmDecl>(outerParams[newIndex]);

        SourceLocation loc = Arg.getLocation();
        if (!loc.isValid())
            return;

        std::string newName = NewParmDecl->getNameAsString();
        Rewrite.ReplaceText(loc, ParmDecl->getNameAsString().size(), newName);
    }
}

void MutatorFrontendAction_61::MutatorASTConsumer_61::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionTemplateDecl(
        has(functionDecl(
            hasBody(compoundStmt(
                has(lambdaExpr(
                    has(cxxMethodDecl(
                        ofClass(isLambda()),
                        hasParameter(0, parmVarDecl(
                            hasType(clang::ast_matchers::constrainedType().bind("constrainedType"))
                        ).bind("lambdaParam"))
                    ).bind("lambdaCallOp"))
                ).bind("lambda")
            ))))
        ).bind("functionTemplate");

    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}