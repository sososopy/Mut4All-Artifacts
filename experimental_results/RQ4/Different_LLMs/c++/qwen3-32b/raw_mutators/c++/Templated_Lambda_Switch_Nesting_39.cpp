//header file
#pragma once
#include "Mutator_base.h"

/**
 * Templated_Lambda_Switch_Nesting_39
 */ 
class MutatorFrontendAction_39 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(39)
private:
    class MutatorASTConsumer_39 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_39(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<VarDecl*> otherLambdas;
    };
};

//source file
#include "../include/Templated_Lambda_Switch_Nesting_39.h"

// ========================================================================================================
#define MUT39_OUTPUT 1

void MutatorFrontendAction_39::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FTD = Result.Nodes.getNodeAs<FunctionTemplateDecl>("lambdaTemplate")) {
        if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(FTD->getLocation()))
            return;

        const FunctionDecl *FD = FTD->getTemplatedDecl();
        if (!FD || !FD->isLambda())
            return;

        const auto *Params = FD->parameters();
        if (Params->size() < 1)
            return;

        const ParmVarDecl *ParamX = Params->front();
        std::string ParamName = ParamX->getNameAsString();

        const TemplateParameterList *TPLambda = FTD->getTemplateParameterList();
        if (!TPLambda || TPLambda->size() < 1)
            return;

        const TemplateParameter *Param = TPLambda->getParam(0);
        const NonTypeTemplateParmDecl *NParam = dyn_cast<NonTypeTemplateParmDecl>(Param);
        if (!NParam)
            return;
        std::string NParamName = NParam->getNameAsString();

        if (otherLambdas.empty())
            return;
        const VarDecl *MVarDecl = otherLambdas[0];
        std::string MName = MVarDecl->getNameAsString();

        const Stmt *Body = FD->getBody();
        if (!Body)
            return;

        SourceLocation BodyLoc = Body->getBeginLoc();
        std::string BodyText = stringutils::rangetoStr(*(Result.SourceManager), Body->getSourceRange());

        std::string SwitchCode = "switch (" + ParamName + ") {\n";
        SwitchCode += "    case 0: " + MName + ".operator()<" + NParamName + ", 0>();\n";
        SwitchCode += "    case 1: " + MName + ".operator()<" + NParamName + ", 1>();\n";
        SwitchCode += "}\n";

        size_t InsertPos = BodyText.find("{") + 1;
        if (InsertPos != std::string::npos) {
            BodyText.insert(InsertPos, SwitchCode);
            Rewrite.ReplaceText(CharSourceRange::getTokenRange(Body->getSourceRange()), BodyText);
        }
    } else if (auto *VD = Result.Nodes.getNodeAs<VarDecl>("otherLambdaVar")) {
        if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(VD->getLocation()))
            return;
        if (const LambdaExpr *LE = VD->getInit()->getAs<LambdaExpr>()) {
            otherLambdas.push_back(VD);
        }
    }
}
  
void MutatorFrontendAction_39::MutatorASTConsumer_39::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher targetMatcher = functionTemplateDecl(hasBody(functionDecl(isLambda()))).bind("lambdaTemplate");
    DeclarationMatcher otherLambdaMatcher = varDecl(hasInitializer(lambdaExpr())).bind("otherLambdaVar");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(targetMatcher, &callback);
    matchFinder.addMatcher(otherLambdaMatcher, &callback);
    matchFinder.matchAST(Context);
}