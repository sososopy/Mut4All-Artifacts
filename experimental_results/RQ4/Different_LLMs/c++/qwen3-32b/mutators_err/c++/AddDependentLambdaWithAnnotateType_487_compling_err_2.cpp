//header file
#pragma once
#include "Mutator_base.h"

/**
 * AddDependentLambdaWithAnnotateType_487
 */ 
class MutatorFrontendAction_487 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(487)

private:
    class MutatorASTConsumer_487 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_487(Rewriter &R) : TheRewriter(R) {}
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
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/AddDependentLambdaWithAnnotateType_487.h"

// ========================================================================================================
#define MUT487_OUTPUT 1

void MutatorFrontendAction_487::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *lambda = Result.Nodes.getNodeAs<clang::LambdaExpr>("lambda")) {
        // Filter nodes in header files
        if (!lambda || !Result.Context->getSourceManager().isWrittenInMainFile(lambda->getBeginLoc()))
            return;

        // Get the enclosing FunctionDecl
        auto *FD = Result.Context->getLambdaClass(lambda)->getParent();
        while (FD && !isa<FunctionDecl>(FD)) {
            FD = FD->getParent();
        }
        FunctionDecl *funcDecl = dyn_cast<FunctionDecl>(FD);
        if (!funcDecl)
            return;

        // Check if the FunctionDecl is part of a FunctionTemplateDecl
        FunctionTemplateDecl *funcTemplate = funcDecl->getDescribedFunctionTemplate();
        if (!funcTemplate)
            return;

        // Get the first template parameter
        TemplateParameterList *tmplParams = funcTemplate->getTemplateParameters();
        if (!tmplParams || tmplParams->size() == 0)
            return;
        TemplateTypeParmDecl *tmplParam = dyn_cast<TemplateTypeParmDecl>(tmplParams->getParam(0));
        if (!tmplParam)
            return;
        std::string paramTypeName = tmplParam->getNameAsString();

        // Replace each parameter's type with the template type
        auto *callOp = lambda->getCallOperator();
        if (!callOp)
            return;
        for (auto *param : callOp->parameters()) {
            if (!param)
                continue;
            SourceLocation typeBegin = param->getTypeSourceInfo()->getTypeLoc().getBeginLoc();
            SourceLocation typeEnd = param->getTypeSourceInfo()->getTypeLoc().getEndLoc();
            if (typeBegin.isInvalid() || typeEnd.isInvalid())
                continue;
            Rewrite.ReplaceText(SourceRange(typeBegin, typeEnd), paramTypeName);
        }

        // Add the annotate_type attribute
        SourceLocation afterParams = lambda->getTypeSourceInfo()->getTypeLoc().getEndLoc();
        if (afterParams.isInvalid())
            afterParams = lambda->getBody()->getBeginLoc();
        Rewrite.InsertText(afterParams, " [[clang::annotate_type(\"example\")]]", true, true);

        // Wrap the lambda in (void) if not already present
        SourceLocation lambdaStart = lambda->getBeginLoc();
        SourceLocation lambdaEnd = lambda->getEndLoc();
        std::string lambdaText = Rewrite.getRewrittenText(SourceRange(lambdaStart, lambdaEnd));
        if (lambdaText.find("(void)") == std::string::npos) {
            Rewrite.InsertText(lambdaStart, "(void)", true, true);
        }
    }
}
  
void MutatorFrontendAction_487::MutatorASTConsumer_487::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    // Define ASTMatchers to identify the target AST node for mutation
    DeclarationMatcher matcher = functionDecl(isTemplateInstantiation(), hasDescendant(lambdaExpr())).bind("lambda");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}