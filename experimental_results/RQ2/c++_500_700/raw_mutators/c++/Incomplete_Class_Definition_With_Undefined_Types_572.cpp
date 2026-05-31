//header file
#pragma once
#include "Mutator_base.h"

/**
 * incomplete_class_definition_with_undefined_types_572
 */ 
class MutatorFrontendAction_572 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(572)

private:
    class MutatorASTConsumer_572 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_572(Rewriter &R) : TheRewriter(R) {}
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
    };
};

//source file
#include "../include/incomplete_class_definition_with_undefined_types_572.h"

// ========================================================================================================
#define MUT572_OUTPUT 1

void MutatorFrontendAction_572::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CRD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("ClassDecl")) {
        //Filter nodes in header files
        if (!CRD || !Result.Context->getSourceManager().isWrittenInMainFile(
                       CRD->getLocation()))
            return;
        if (!CRD->hasDefinition() || CRD->isLambda())
            return;
        
        //Get the source code text of target node
        auto className = CRD->getNameAsString();
        std::string mutatedCode = "class " + className + " : public UndefinedType, AnotherUndefinedType\n";
        
        //Perform mutation on the source code text by applying string replacement
        Rewrite.ReplaceText(CRD->getSourceRange(), mutatedCode);
    }
}
  
void MutatorFrontendAction_572::MutatorASTConsumer_572::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define an ASTMatcher to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl(isClass()).bind("ClassDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}