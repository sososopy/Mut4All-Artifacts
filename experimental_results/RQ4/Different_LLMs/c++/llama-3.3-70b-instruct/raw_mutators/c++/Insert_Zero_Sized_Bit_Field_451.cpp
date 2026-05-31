//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Zero_Sized_Bit_Field_451
 */ 
class MutatorFrontendAction_451 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(451)

private:
    class MutatorASTConsumer_451 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_451(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Mutator_Insert_Zero_Sized_Bit_Field_451.h"

// ========================================================================================================
#define MUT451_OUTPUT 1

void MutatorFrontendAction_451::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *RD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("RecordDecl")) {
      //Filter nodes in header files
      if (!RD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     RD->getLocation()))
        return;
      //Get the source code text of target node
      auto content = stringutils::rangetoStr(*(Result.SourceManager), RD->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      size_t pos = content.find("}");
      if (pos != string::npos) {
        content.insert(pos, "\n  unsigned : 0;");
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(RD->getSourceRange()), content);
    }
}

void MutatorFrontendAction_451::MutatorASTConsumer_451::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl(hasAnyMember(hasType(recordType(hasMember(fieldDecl(hasType(isInteger())))))).bind("RecordDecl"));
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}