//header file
#pragma once
#include "Mutator_base.h"

/**
 * Nested_Name_Specifier_With_Pointer_To_Member_413
 */ 
class MutatorFrontendAction_413 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(413)

private:
    class MutatorASTConsumer_413 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_413(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/nested_name_specifier_with_pointer_to_member_413.h"

// ========================================================================================================
#define MUT413_OUTPUT 1

void MutatorFrontendAction_413::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::MemberPointerType>("MemberPointer")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto type = stringutils::rangetoStr(*(Result.SourceManager),
                                          MT->getSourceRange());
      llvm::outs() << "type: " << type << '\n';
      //Perform mutation on the source code text by applying string replacement
      auto ins = "typename A::B::C::D::E::F::G::H::" + type;
      llvm::outs() << "ins: " << ins << '\n';
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(MT->getSourceRange(), ins);
    }
}
  
void MutatorFrontendAction_413::MutatorASTConsumer_413::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = memberPointerType().bind("MemberPointer");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}