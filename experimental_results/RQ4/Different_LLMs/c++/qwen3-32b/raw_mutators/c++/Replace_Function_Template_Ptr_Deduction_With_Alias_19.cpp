//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Function_Template_Ptr_Deduction_With_Alias_19
 */ 
class MutatorFrontendAction_19 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(19)
private:
    class MutatorASTConsumer_19 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_19(Rewriter &R) : TheRewriter(R) {}
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
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Replace_Function_Template_Ptr_Deduction_With_Alias_19.h"

// ========================================================================================================
#define MUT19_OUTPUT 1

void MutatorFrontendAction_19::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *VD = Result.Nodes.getNodeAs<clang::VarDecl>("var")) {
        if (!VD || !Result.Context->getSourceManager().isWrittenInMainFile(VD->getLocation()))
            return;

        auto *Init = VD->getInit();
        if (!Init)
            return;

        auto *UnaryOp = Init->IgnoreImpCasts()->getAs<clang::UnaryOperator>();
        if (!UnaryOp || UnaryOp->getOpcode() != clang::UO_AddrOf)
            return;

        auto *DRE = UnaryOp->getSubExpr()->IgnoreImpCasts()->getAs<clang::DeclRefExpr>();
        if (!DRE)
            return;

        auto *FD = dyn_cast<clang::FunctionDecl>(DRE->getDecl());
        if (!FD || !FD->getDescribedFunctionTemplate())
            return;

        auto *FTD = FD->getDescribedFunctionTemplate();
        if (!FTD)
            return;

        SourceManager &SM = Result.Context->getSourceManager();
        std::string templateDeclStr = stringutils::rangetoStr(SM, FTD->getSourceRange());
        if (templateDeclStr.empty())
            return;

        std::string funcName = FD->getNameAsString();

        std::string templateArgsStr;
        for (const auto *Param : FTD->getTemplateParameters()->asRange()) {
            if (!templateArgsStr.empty())
                templateArgsStr += ", ";
            templateArgsStr += Param->getNameAsString();
        }

        std::string aliasDecl = templateDeclStr + " using FuncAlias = decltype(" + funcName + "<" + templateArgsStr + ">(0));";

        std::string templateArgsInstantiation;
        for (unsigned i = 0; i < FD->getTemplateArgs().size(); ++i) {
            if (i > 0)
                templateArgsInstantiation += ", ";
            templateArgsInstantiation += FD->getTemplateArgs().get(i).getAsType().getAsString();
        }

        std::string newVarDecl = "FuncAlias<" + templateArgsInstantiation + ">* " + VD->getNameAsString() + " = &" + funcName + "<" + templateArgsInstantiation + ">;";

        SourceLocation loc = VD->getLocation();
        Rewrite.InsertText(loc, aliasDecl + "\n", true, true);

        CharSourceRange range = CharSourceRange::getTokenRange(VD->getSourceRange());
        Rewrite.ReplaceText(range, newVarDecl);
    }
}
  
void MutatorFrontendAction_19::MutatorASTConsumer_19::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = varDecl(
        hasType(decltypeAutoType()),
        hasInitializer(ignoringParenImpCasts(
            unaryOperator(
                hasOperatorName("&"),
                hasUnaryOperand(
                    declRefExpr(
                        to(functionDecl(isFunctionTemplate()))
                    )
                )
            )
        )),
        unless(isLocalVarDecl())
    ).bind("var");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}