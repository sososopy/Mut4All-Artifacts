//header file
#pragma once
#include "Mutator_base.h"

/**
 * modify_class_destructors_with_defaulted_specifiers_37
 */ 
class MutatorFrontendAction_37 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(37)

private:
    class MutatorASTConsumer_37 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_37(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/modify_class_destructors_with_defaulted_specifiers_37.h"

// ========================================================================================================
#define MUT37_OUTPUT 1

void MutatorFrontendAction_37::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
        //Filter nodes in header files
        if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(
                       CL->getLocation()))
            return;

        if (!CL->isCompleteDefinition())
            return;

        auto *Destructor = CL->getDestructor();
        if (Destructor) {
            if (!Destructor->isDefaulted()) {
                //Get the source code text of target node
                auto dtorRange = Destructor->getSourceRange();
                std::string dtorText = stringutils::rangetoStr(*(Result.SourceManager), dtorRange);
                //Perform mutation on the source code text by applying string replacement
                std::string defaultedDtor = "~" + CL->getNameAsString() + "() = default;";
                //Replace the original AST node with the mutated one
                Rewrite.ReplaceText(CharSourceRange::getTokenRange(dtorRange), defaultedDtor);
            }
        } else {
            // If destructor does not exist, create a defaulted one
            std::string defaultedDtor = "\n~" + CL->getNameAsString() + "() = default;";
            Rewrite.InsertTextAfterToken(CL->getRBraceLoc(), defaultedDtor);
        }
    }
}
  
void MutatorFrontendAction_37::MutatorASTConsumer_37::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl().bind("Classes");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}