//header file
#pragma once
#include "Mutator_base.h"

/**
 * Overloaded_Function_with_Attribute_Aggression_443
 */ 
class MutatorFrontendAction_443 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(443)

private:
    class MutatorASTConsumer_443 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_443(Rewriter &R) : TheRewriter(R) {}
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
        std::map<std::string, std::vector<const clang::FunctionDecl *>>
            overloads_map;
    };
};

//source file
#include "../include/Overloaded_Function_with_Attribute_Aggression_443.h"

// ========================================================================================================
#define MUT443_OUTPUT 1

void MutatorFrontendAction_443::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->isOverloadedOperator())
        return;
      auto func_name = FD->getNameAsString();
      overloads_map[func_name].push_back(FD);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::Decl>("Decls")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      for (auto it : overloads_map) {
        if (it.second.size() == 1)
          continue;
        int index = getrandom::getRandomIndex(it.second.size() - 1);
        auto target = it.second[index];
        if (target->isCXXClassMember())
          continue;
        if (!target->hasBody())
          continue;
        auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                               target->getSourceRange());
        if (content.find("inline") == string::npos)
          content = "inline " + content;
        content = "[[clang::always_inline]] " + content;
        auto body = stringutils::rangetoStr(*(Result.SourceManager),
                                            target->getBody()->getSourceRange());
        auto call = it.first + "(";
        for (auto param : target->parameters()) {
          call += param->getNameAsString() + ",";
        }
        if (call.back() == ',')
          call.pop_back();
        call += ");";
        body.insert(body.find("{") + 1, call);
        content.replace(content.find('{'), content.rfind('}') - content.find('{') + 1, body);
        Rewrite.ReplaceText(CharSourceRange::getTokenRange(target->getSourceRange()), content);
      }
    }
  }
  
void MutatorFrontendAction_443::MutatorASTConsumer_443::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = functionDecl().bind("Functions");
    auto decl_matcher = decl().bind("Decls");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(decl_matcher, &callback);
    matchFinder.matchAST(Context);
}