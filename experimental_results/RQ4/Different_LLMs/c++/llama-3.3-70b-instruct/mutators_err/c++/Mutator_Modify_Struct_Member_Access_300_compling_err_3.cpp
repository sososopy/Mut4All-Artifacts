//header file
#pragma once
#include "Mutator_base.h"

/**
 * Modify_Struct_Member_Access_300
 */ 
class MutatorFrontendAction_300 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(300)

private:
    class MutatorASTConsumer_300 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_300(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        virtual void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        //Necessary node information record used in the mutation process
    };
};

//source file
#include "../include/Mutator_Modify_Struct_Member_Access_300.h"

// ========================================================================================================
#define MUT300_OUTPUT 1

void MutatorFrontendAction_300::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::MemberExpr>(("MemberExpr"))) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isInMainFile(MT->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                   MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      std::string structName = MT->getBase()->getType().getAsString();
      const clang::RecordType *recordType = MT->getBase()->getType()->getAs<clang::RecordType>();
      if (recordType) {
        std::vector<std::string> memberNames;
        for (auto &field : recordType->fields()) {
          memberNames.push_back(field.getNameAsString());
        }
        int choice = getrandom::getRandomIndex(memberNames.size());
        std::string mutatedMemberName = memberNames[choice];
        std::string mutatedDeclaration = declaration;
        size_t dotPos = declaration.find('.');
        if (dotPos != std::string::npos) {
          mutatedDeclaration.replace(dotPos + 1, MT->getMemberDecl()->getNameAsString().size(), mutatedMemberName);
        }
        //Replace the original AST node with the mutated one
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), mutatedDeclaration);
      }
    }
}
  
void MutatorFrontendAction_300::MutatorASTConsumer_300::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    StatementMatcher matcher = memberExpr(hasType(recordType())).bind("MemberExpr");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}