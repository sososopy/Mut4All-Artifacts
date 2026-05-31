//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Function_With_Deleted_Template_Using_Invalid_Type_Parameter_387
 */ 
class MutatorFrontendAction_Replace_Function_With_Deleted_Template_Using_Invalid_Type_Parameter_387 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(Replace_Function_With_Deleted_Template_Using_Invalid_Type_Parameter_387)
private:
    class MutatorASTConsumer_Replace_Function_With_Deleted_Template_Using_Invalid_Type_Parameter_387 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_Replace_Function_With_Deleted_Template_Using_Invalid_Type_Parameter_387(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Replace_Function_With_Deleted_Template_Using_Invalid_Type_Parameter_387.h"

// ========================================================================================================
#define MUTReplace_Function_With_Deleted_Template_Using_Invalid_Type_Parameter_387_OUTPUT 1

void MutatorFrontendAction_Replace_Function_With_Deleted_Template_Using_Invalid_Type_Parameter_387::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::FunctionDecl>("functionDecl")) {
        if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(MT->getLocation()))
            return;

        std::string funcName = MT->getNameAsString();
        if (funcName.empty())
            return;

        std::string paramsStr;
        for (const auto *param : MT->parameters()) {
            if (!paramsStr.empty())
                paramsStr += ", ";
            paramsStr += param->getType().getAsString() + " " + param->getNameAsString();
        }

        std::string templateFunc = "template <" + funcName + "> auto " + funcName + "(" + paramsStr + ") = delete;\n";
        std::string decltypeLine = "decltype(" + funcName + "<0>()) x;\n";
        std::string newCode = templateFunc + decltypeLine;

        Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), newCode);
    }
}
  
void MutatorFrontendAction_Replace_Function_With_Deleted_Template_Using_Invalid_Type_Parameter_387::MutatorASTConsumer_Replace_Function_With_Deleted_Template_Using_Invalid_Type_Parameter_387::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = functionDecl().bind("functionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}