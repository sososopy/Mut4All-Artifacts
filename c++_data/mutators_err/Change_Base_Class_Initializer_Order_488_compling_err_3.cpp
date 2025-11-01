//header file
#pragma once
#include "Mutator_base.h"

/**
 * Change_Base_Class_Initializer_Order_488
 */ 
class MutatorFrontendAction_488 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(488)

private:
    class MutatorASTConsumer_488 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_488(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Change_Base_Class_Initializer_Order_488.h"

// ========================================================================================================
#define MUT488_OUTPUT 1

void MutatorFrontendAction_488::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *Ctor = Result.Nodes.getNodeAs<clang::CXXConstructorDecl>("DerivedCtor")) {
        //Filter nodes in header files
        if (!Ctor || !Result.Context->getSourceManager().isWrittenInMainFile(Ctor->getLocation()))
            return;

        //Get the source code text of target node
        auto initList = Ctor->inits();
        if (std::distance(initList.begin(), initList.end()) < 2)
            return; // No need to reorder if less than 2 initializers

        //Perform mutation on the source code text by applying string replacement
        std::vector<const CXXCtorInitializer*> reorderedInitList(initList.begin(), initList.end());
        std::swap(reorderedInitList[0], reorderedInitList[1]); // Swap the first two initializers as an example

        std::string newInitListText;
        for (const auto *init : reorderedInitList) {
            if (!newInitListText.empty()) {
                newInitListText += ", ";
            }
            newInitListText += Lexer::getSourceText(CharSourceRange::getTokenRange(init->getSourceRange()), 
                                                    *Result.SourceManager, 
                                                    Result.Context->getLangOpts()).str();
        }

        SourceLocation startLoc = initList.begin()->getSourceLocation();
        SourceLocation endLoc = (initList.end() - 1)->getSourceRange().getEnd();
        Rewrite.ReplaceText(SourceRange(startLoc, endLoc), newInitListText);
    }
}
  
void MutatorFrontendAction_488::MutatorASTConsumer_488::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxConstructorDecl(ofClass(cxxRecordDecl(isDerivedFrom(hasAnyName("Base1", "Base2")))))
                                 .bind("DerivedCtor");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}