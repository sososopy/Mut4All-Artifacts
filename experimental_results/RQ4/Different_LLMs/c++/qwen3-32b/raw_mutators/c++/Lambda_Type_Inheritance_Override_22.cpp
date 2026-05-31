//header file
#pragma once
#include "Mutator_base.h"

/**
 * Lambda_Type_Inheritance_Override_22
 */ 
class MutatorFrontendAction_22 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(22)

private:
    class MutatorASTConsumer_22 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_22(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Lambda_Type_Inheritance_Override_22.h"

// ========================================================================================================
#define MUT22_OUTPUT 1

void MutatorFrontendAction_22::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("methodDecl")) {
        // Filter nodes in header files
        if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(MT->getLocation()))
            return;

        // Get the source manager
        SourceManager &SM = *Result.SourceManager;

        // Modify the template parameters
        if (auto *TPL = MT->getTemplateParameterList()) {
            SourceLocation tplStart = TPL->getTemplateLoc();
            SourceLocation tplEnd = TPL->getSourceRange().getEnd();
            std::string tplText = stringutils::rangetoStr(SM, SourceRange(tplStart, tplEnd));
            std::string newTpl = "typename T, " + tplText;
            Rewrite.ReplaceText(SourceRange(tplStart, tplEnd), newTpl);
        }

        // Modify the parameter list
        SourceLocation lParenLoc = MT->getLParenLoc();
        SourceLocation rParenLoc = MT->getRParenLoc();
        std::string paramText = stringutils::rangetoStr(SM, CharSourceRange::getCharRange(lParenLoc, rParenLoc));
        if (paramText.length() >= 2 && paramText[0] == '(' && paramText[paramText.length()-1] == ')') {
            paramText = paramText.substr(1, paramText.length()-2);
        }
        std::string newParamText = "T t, " + paramText;
        Rewrite.ReplaceText(CharSourceRange::getCharRange(lParenLoc, rParenLoc), "(" + newParamText + ")");
    }
}

void MutatorFrontendAction_22::MutatorASTConsumer_22::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxMethodDecl(
        hasName("operator()"),
        ofClass(cxxRecordDecl(hasAnyBase(hasType(cxxRecordDecl(isLambda()))))),
        hasTrailingReturnType()
    ).bind("methodDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}