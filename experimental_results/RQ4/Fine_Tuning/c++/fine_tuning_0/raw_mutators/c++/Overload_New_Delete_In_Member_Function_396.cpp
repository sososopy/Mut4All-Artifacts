//header file
#pragma once
#include "Mutator_base.h"

/**
 * Overload_New_Delete_In_Member_Function_396
 */ 
class MutatorFrontendAction_396 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(396)

private:
    class MutatorASTConsumer_396 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_396(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Overload_New_Delete_In_Member_Function_396.h"

// ========================================================================================================
#define MUT396_OUTPUT 1

void MutatorFrontendAction_396::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
      //Filter nodes in header files
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      //Get the source code text of target node
      auto declaration = stringutils::rangetoStr(*(Result.SourceManager),
                                                 MT->getSourceRange());
      //Perform mutation on the source code text by applying string replacement
      if (MT->isCompleteDefinition() == false)
        return;
      if (MT->isLambda())
        return;
      if (MT->isStruct() == false && MT->isClass() == false)
        return;
      auto methods = MT->methods();
      if (methods.empty()) {
        auto name = "mut_396";
        auto rettype = "void";
        auto method_decl = rettype + " " + name + "(){}";
        declaration.insert(declaration.rfind('}'), method_decl);
      } else {
        for (auto method : methods) {
          if (method->hasBody()) {
            auto body = stringutils::rangetoStr(*(Result.SourceManager),
                                                method->getBody()->getSourceRange());
            auto new_overload = "void *operator new(decltype(sizeof(int)));";
            auto delete_overload = "void foo() { delete (int *)0; };";
            body.insert(1, "/*mut396*/" + new_overload + delete_overload);
            declaration.replace(declaration.find('{', method->getBeginLoc().getRawEncoding() -
                                                             MT->getBeginLoc().getRawEncoding()),
                                body.size(), body);
            break;
          }
        }
      }
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(MT->getSourceRange()), declaration);
    }
}
  
void MutatorFrontendAction_396::MutatorASTConsumer_396::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    DeclarationMatcher matcher = cxxRecordDecl().bind("Classes");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}