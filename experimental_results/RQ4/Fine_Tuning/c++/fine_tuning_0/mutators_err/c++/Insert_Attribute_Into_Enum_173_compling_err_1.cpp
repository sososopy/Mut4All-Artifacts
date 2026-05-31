//header file
#pragma once
#include "Mutator_base.h"

/**
 * Insert_Attribute_Into_Enum_173
 */ 
class MutatorFrontendAction_173 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(173)

private:
    class MutatorASTConsumer_173 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_173(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::Attr *> cur_attrs;
    };
};

//source file
#include "../include/Insert_Attribute_Into_Enum_173.h"

// ========================================================================================================
#define MUT173_OUTPUT 1

void MutatorFrontendAction_173::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *AT = Result.Nodes.getNodeAs<clang::Attr>("Attributes")) {
      //Filter nodes in header files
      if (!AT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     AT->getLocation()))
        return;
      //Record the candidate existing attributes for insertion
      cur_attrs.push_back(AT);
    } else if (auto *ET =
                   Result.Nodes.getNodeAs<clang::EnumDecl>("Enums")) {
      if (!ET || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ET->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                 ET->getSourceRange());
      string attrname;
      if (!cur_attrs.empty()) {
        auto index = getrandom::getRandomIndex(cur_attrs.size() - 1);
        attrname = stringutils::rangetoStr(*(Result.SourceManager),
                                           cur_attrs[index]->getSourceRange());
      } else {
        attrname = "[[nodiscard]]";
      }
      //Perform mutation on the source code text by applying string replacement
      declaration = attrname + " " + declaration;
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(ET->getSourceRange(), declaration);
    }
}
  
void MutatorFrontendAction_173::MutatorASTConsumer_173::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto attr_matcher = attr().bind("Attributes");
    auto enum_matcher = enumDecl().bind("Enums");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(attr_matcher, &callback);
    matchFinder.addMatcher(enum_matcher, &callback);
    matchFinder.matchAST(Context);
}