//header file
#pragma once
#include "Mutator_base.h"

/**
 * CRTP_Consteval_Mutation_59
 */ 
class MutatorFrontendAction_59 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(59)

private:
    class MutatorASTConsumer_59 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_59(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::CXXRecordDecl *> cur_classes;
    };
};

//source file
#include "../include/CRTP_Consteval_Mutation_59.h"

// ========================================================================================================
#define MUT59_OUTPUT 1

void MutatorFrontendAction_59::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
      if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CL->getLocation()))
        return;
      if ((!CL->isStruct() && !CL->isClass()) || CL->isLambda())
        return;
      if (!CL->isCompleteDefinition())
        return;
      if (CL->isAbstract())
        return;
      cur_classes.push_back(CL);
    } else if (auto *CF = Result.Nodes.getNodeAs<clang::FunctionDecl>("Consteval")) {
      if (!CF || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CF->getLocation()))
        return;
      if (CF->isConsteval() == false)
        return;
      if (!CF->hasBody())
        return;
      if (CF->isLambdaStaticInvoker())
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             CF->getSourceRange());
      auto body = CF->getBody();
      auto body_content = stringutils::rangetoStr(*(Result.SourceManager),
                                                  body->getSourceRange());
      llvm::outs() << body_content;
      if (body_content.find("static_cast") == string::npos &&
          body_content.find("dynamic_cast") == string::npos)
        return;
      int choice = getrandom::getRandomIndex(1);
      if (choice == 0) {
        if (body_content.find("static_cast") != string::npos)
          body_content.replace(body_content.find("static_cast"),
                               sizeof("static_cast") - 1, "dynamic_cast");
        else
          body_content.replace(body_content.find("dynamic_cast"),
                               sizeof("dynamic_cast") - 1, "static_cast");
      } else {
        auto target = cur_classes[getrandom::getRandomIndex(cur_classes.size())];
        if (body_content.find("static_cast") != string::npos)
          body_content.replace(body_content.find("static_cast"),
                               sizeof("static_cast") - 1, "reinterpret_cast");
        else
          body_content.replace(body_content.find("dynamic_cast"),
                               sizeof("dynamic_cast") - 1, "reinterpret_cast");
        body_content.insert(body_content.find('(') + 1,
                            target->getNameAsString() + "*");
        llvm::outs() << body_content;
      }
      content.replace(content.find('{'), content.rfind('}') - content.find('{'),
                      body_content);
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(CF->getSourceRange()),
                          content);
    }
  }
  
void MutatorFrontendAction_59::MutatorASTConsumer_59::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxRecordDecl().bind("Classes");
    auto consteval_matcher = functionDecl().bind("Consteval");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(consteval_matcher, &callback);
    matchFinder.matchAST(Context);
}