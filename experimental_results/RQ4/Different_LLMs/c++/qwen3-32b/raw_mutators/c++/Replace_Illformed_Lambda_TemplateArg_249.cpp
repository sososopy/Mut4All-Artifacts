//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Illformed_Lambda_TemplateArg_249
 */ 
class MutatorFrontendAction_249 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(249)

private:
    class MutatorASTConsumer_249 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_249(Rewriter &R) : TheRewriter(R) {}
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
        std::set<const clang::FunctionTemplateDecl*> targetTemplates;
    };
};

//source file
#include "../include/Replace_Illformed_Lambda_TemplateArg_249.h"

// ========================================================================================================
#define MUT249_OUTPUT 1

void MutatorFrontendAction_249::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("funcTemplate")) {
        if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(FTD->getLocation()))
            return;
        targetTemplates.insert(FTD);
    } else if (auto *CE = Result.Nodes.getNodeAs<clang::CallExpr>("callExpr")) {
        if (!CE || !Result.Context->getSourceManager().isWrittenInMainFile(CE->getLocation()))
            return;
        auto *FTSD = CE->getCallee()->getAs<clang::FunctionTemplateSpecializationExpr>();
        if (!FTSD)
            return;
        auto *FTD = FTSD->getTemplateName().getAsTemplateDecl<clang::FunctionTemplateDecl>();
        if (!FTD || targetTemplates.find(FTD) == targetTemplates.end())
            return;
        for (unsigned i = 0; i < CE->getNumArgs(); ++i) {
            auto *Arg = CE->getArg(i);
            auto *Lambda = Arg->IgnoreImpCasts()->getAs<clang::LambdaExpr>();
            if (Lambda) {
                SourceLocation Start = Lambda->getBeginLoc();
                SourceLocation End = Lambda->getEndLoc();
                if (Start.isInvalid() || End.isInvalid())
                    continue;
                std::string LambdaText = stringutils::rangetoStr(*Result.SourceManager, CharSourceRange::getTokenRange(Start, End));
                size_t captureStart = LambdaText.find('[');
                if (captureStart == std::string::npos)
                    continue;
                size_t captureEnd = LambdaText.find(']');
                if (captureEnd == std::string::npos)
                    continue;
                std::string newCapture = "[x=0]";
                std::string newLambdaText = newCapture + LambdaText.substr(captureEnd + 1);
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(Start, End), newLambdaText);
                break;
            }
        }
    }
}
  
void MutatorFrontendAction_249::MutatorASTConsumer_249::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher funcTemplateMatcher = functionTemplateDecl(
        hasDescendant(sizeOfExpr(has(declRefExpr(to(decl(parmVarDecl())))))),
        hasAnyParameter(parmVarDecl(hasType(templateTypeParmType())))
    ).bind("funcTemplate");
    DeclarationMatcher callMatcher = callExpr(
        callee(functionTemplateSpecializationDecl()),
        hasArgument(0, expr(ignoringImpCasts(lambdaExpr().bind("lambda"))))
    ).bind("callExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(funcTemplateMatcher, &callback);
    matchFinder.addMatcher(callMatcher, &callback);
    matchFinder.matchAST(Context);
}