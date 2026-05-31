//header file
#pragma once
#include "Mutator_base.h"

/**
 * Enum_Class_Misuse_80
 */ 
class MutatorFrontendAction_80 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(80)

private:
    class MutatorASTConsumer_80 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_80(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Enum_Class_Misuse_80.h"

// ========================================================================================================
#define MUT80_OUTPUT 1

void MutatorFrontendAction_80::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *ED = Result.Nodes.getNodeAs<clang::EnumDecl>("EnumDecl")) {
      //Filter nodes in header files
      if (!ED || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ED->getLocation()))
        return;

      if (ED->isScoped()) {
        //Get the source code text of target node
        auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   ED->getSourceRange());
        //Perform mutation on the source code text by applying string replacement
        std::string misuse = "\nvoid misusedFunction() {\n    using enum; // Incorrect usage\n}\n";
        declaration += misuse;
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(ED->getSourceRange()), declaration);
      }
    }
}
  
void MutatorFrontendAction_80::MutatorASTConsumer_80::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = enumDecl(isScoped()).bind("EnumDecl");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}