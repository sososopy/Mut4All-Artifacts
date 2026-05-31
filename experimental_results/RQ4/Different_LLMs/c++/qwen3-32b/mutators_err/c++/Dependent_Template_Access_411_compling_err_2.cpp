//header file
#pragma once
#include "Mutator_base.h"

/**
 * Dependent_Template_Access_411
 */ 
class MutatorFrontendAction_411 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(411)

private:
    class MutatorASTConsumer_411 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_411(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Dependent_Template_Access_411.h"

// ========================================================================================================
#define MUT411_OUTPUT 1

void MutatorFrontendAction_411::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DRE = Result.Nodes.getNodeAs<clang::DeclRefExpr>("declRefExprNode")) {
        if (!DRE || !Result.Context->getSourceManager().isWrittenInMainFile(DRE->getLocation()))
            return;

        const clang::NestedNameSpecifier *NNS = DRE->getQualifier();
        if (!NNS || !NNS->isDependent())
            return;

        const clang::DeclContext *DC = DRE->getDecl()->getDeclContext();
        const clang::TemplateParameterList *TPL = nullptr;
        while (DC && !TPL) {
            if (const auto *FTD = clang::dyn_cast<clang::FunctionTemplateDecl>(DC))
                TPL = FTD->getTemplateParameters();
            else if (const auto *CTD = clang::dyn_cast<clang::ClassTemplateDecl>(DC))
                TPL = CTD->getTemplateParameters();
            else
                DC = DC->getParent();
        }

        if (!TPL || TPL->size() == 0)
            return;

        const clang::NamedDecl *TParamDecl = TPL->getParam(0);
        const clang::TemplateTypeParmDecl *TParam = clang::dyn_cast<clang::TemplateTypeParmDecl>(TParamDecl);
        if (!TParam)
            return;

        std::string TName = TParam->getNameAsString();

        SourceLocation StartLoc = DRE->getBeginLoc();
        SourceLocation EndLoc = DRE->getEndLoc();
        std::string originalCode = stringutils::rangetoStr(*Result.SourceManager, CharSourceRange::getTokenRange(StartLoc, EndLoc).getAsRange());

        size_t lastColon = originalCode.rfind("::");
        if (lastColon == std::string::npos)
            return;

        std::string qualifierPart = originalCode.substr(0, lastColon);
        std::string memberPart = originalCode.substr(lastColon);

        size_t firstColon = qualifierPart.find("::");
        if (firstColon == std::string::npos)
            return;

        std::string firstPart = qualifierPart.substr(0, firstColon);
        std::string restPart = qualifierPart.substr(firstColon + 2);

        std::string newQualifier = "typename " + firstPart + "::template Nested<" + TName + ">::template " + restPart;
        std::string newCode = newQualifier + memberPart;

        Rewrite.ReplaceText(CharSourceRange::getTokenRange(StartLoc, EndLoc).getAsRange(), newCode);
    }
}
  
void MutatorFrontendAction_411::MutatorASTConsumer_411::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = declRefExpr(hasType(type::anything())).bind("declRefExprNode");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}