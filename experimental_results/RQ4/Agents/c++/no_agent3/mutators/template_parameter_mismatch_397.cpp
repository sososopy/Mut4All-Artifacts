//header file
#pragma once
#include "Mutator_base.h"

/**
 * Template_Parameter_Mismatch_397
 */ 
class MutatorFrontendAction_397 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(397)

private:
    class MutatorASTConsumer_397 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_397(Rewriter &R) : TheRewriter(R) {}
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
        const clang::FunctionTemplateDecl *CurrentTemplate = nullptr;
    };
};

//source file
#include "../include/Mutator_397.h"

// ========================================================================================================
#define MUT397_OUTPUT 1

void MutatorFrontendAction_397::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *FTD = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>("FunctionTemplate")) {
        //Filter nodes in header files
        if (!FTD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       FTD->getLocation()))
            return;
        
        CurrentTemplate = FTD;
        //Get the source code text of target node
        auto declaration = stringutils::rangetoStr(*(Result.SourceManager), FTD->getSourceRange());
        
        //Perform mutation on the source code text by applying string replacement
        if (const auto *Function = FTD->getTemplatedDecl()) {
            for (const auto *Param : Function->parameters()) {
                if (const auto *Type = Param->getType()->getAs<clang::BuiltinType>()) {
                    if (Type->isIntegerType()) {
                        std::string mismatchCall = "process<double>(" + Param->getNameAsString() + ");";
                        declaration.insert(declaration.find("{") + 1, "\n/*mut397*/" + mismatchCall + "\n");
                        break;
                    }
                }
            }
        }

        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(FTD->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_397::MutatorASTConsumer_397::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = functionTemplateDecl().bind("FunctionTemplate");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}