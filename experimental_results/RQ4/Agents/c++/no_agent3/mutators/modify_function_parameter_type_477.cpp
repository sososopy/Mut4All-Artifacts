//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Function_Parameter_Type_477
 */ 
class MutatorFrontendAction_477 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(477)

private:
    class MutatorASTConsumer_477 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_477(Rewriter &R) : TheRewriter(R) {}
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
        const ParmVarDecl *firstParam = nullptr;
    };
};

//source file
#include "../include/modify_function_parameter_type_477.h"

// ========================================================================================================
#define MUT477_OUTPUT 1

void MutatorFrontendAction_477::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("FunctionDecl")) {
      //Filter nodes in header files
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      
      if (FD->getNumParams() > 0) {
        firstParam = FD->getParamDecl(0);
        auto paramType = firstParam->getType();
        std::string newTypeStr;
        
        // Modify the type of the first parameter
        if (paramType->isPointerType()) {
          newTypeStr = paramType.getAsString() + "*";
        } else {
          newTypeStr = paramType.getAsString() + "*";
        }

        //Get the source code text of target node
        auto paramRange = firstParam->getSourceRange();
        std::string paramText = stringutils::rangetoStr(*(Result.SourceManager), paramRange);
        
        //Perform mutation on the source code text by applying string replacement
        std::string newParamText = newTypeStr + " " + firstParam->getNameAsString();
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(paramRange), newParamText);
      }
    }
}

void MutatorFrontendAction_477::MutatorASTConsumer_477::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionDecl(hasParameter(0, parmVarDecl().bind("FirstParam"))).bind("FunctionDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}