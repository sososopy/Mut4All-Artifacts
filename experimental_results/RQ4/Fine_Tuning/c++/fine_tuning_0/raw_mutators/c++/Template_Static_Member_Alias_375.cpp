//header file
#pragma once
#include "Mutator_base.h"

/**
 * Template_Static_Member_Alias_375
 */ 
class MutatorFrontendAction_375 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(375)

private:
    class MutatorASTConsumer_375 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_375(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::ClassTemplateDecl *> class_templates;
    };
};

//source file
#include "../include/Template_Static_Member_Alias_375.h"

// ========================================================================================================
#define MUT375_OUTPUT 1

void MutatorFrontendAction_375::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *DL = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("ClassTemplates")) {
      //Filter nodes in header files
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
      if (content.find("static") != string::npos) {
        //Record the candidate class templates for insertion
        class_templates.push_back(DL);
      }
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>(
                   "Structs")) { // Find the structs that are not templates
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getBeginLoc()))
        return;
      if (DL->isTemplated() || DL->isLambda())
        return;
      //Get the source code text of target node
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
      auto name = DL->getNameAsString();
      if (content.find("using") != string::npos)
        return;
      if (class_templates.empty())
        return;
      int index = getrandom::getRandomIndex(class_templates.size() - 1);
      auto target = class_templates[index];
      auto target_name = target->getNameAsString();
      //Perform mutation on the source code text by applying string replacement
      auto ins = "/*mut375*/template <typename T> using I = typename " +
                 target_name + "<T>::v;";
      if (content.rfind('}') != string::npos)
        content.insert(content.rfind('}'), ins);
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(DL->getSourceRange()), content);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>(
                   "Structs2")) { // Find the structs that are not templates
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getBeginLoc()))
        return;
      if (DL->isTemplated() || DL->isLambda())
        return;
      //Get the source code text of target node
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
      auto name = DL->getNameAsString();
      if (content.find("foo") != string::npos)
        return;
      auto ins = "template <typename T> int foo() { return T::template I<int>; }";
      if (content.rfind('}') != string::npos)
        content.insert(content.rfind('}'), ins);
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(DL->getSourceRange()), content);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>(
                   "Funcs")) { // Find the structs that are not templates
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getBeginLoc()))
        return;
      //Get the source code text of target node
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
      if (content.find("foo") != string::npos)
        return;
      auto ins = "Q m; return m.foo<P>();";
      if (content.rfind('}') != string::npos)
        content.insert(content.rfind('}'), ins);
      //Replace the original AST node with the mutated one
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(DL->getSourceRange()), content);
    }
}
  
void MutatorFrontendAction_375::MutatorASTConsumer_375::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto class_template_matcher =
        classTemplateDecl(has(cxxRecordDecl(has(cxxMethodDecl()))))
            .bind("ClassTemplates");
    auto struct_matcher = cxxRecordDecl().bind("Structs");
    auto struct_matcher2 = cxxRecordDecl().bind("Structs2");
    auto func_matcher = functionDecl().bind("Funcs");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(class_template_matcher, &callback);
    matchFinder.addMatcher(struct_matcher, &callback);
    matchFinder.addMatcher(struct_matcher2, &callback);
    matchFinder.addMatcher(func_matcher, &callback);
    matchFinder.matchAST(Context);
}