//header file
#pragma once
#include "Mutator_base.h"

/**
 * Incomplete_Class_Definition_193
 */ 
class MutatorFrontendAction_193 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(193)

private:
    class MutatorASTConsumer_193 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_193(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/incomplete_class_definition_193.h"

// ========================================================================================================
#define MUT193_OUTPUT 1

void MutatorFrontendAction_193::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if ((!DL->isStruct() && !DL->isClass()) || DL->isLambda())
      return;
    if (!DL->isCompleteDefinition())
      return;

    auto content =
        stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
    llvm::outs() << content << '\n';
    auto fields = DL->fields();
    auto methods = DL->methods();
    if (fields.empty() && methods.empty())
      return;
    int choice = getrandom::getRandomIndex(4);
    if (choice == 0) {
      if (fields.empty())
        return;
      int index = getrandom::getRandomIndex(std::distance(fields.begin(), fields.end()) - 1);
      auto target = *(fields.begin() + index);
      auto field_content = stringutils::rangetoStr(*(Result.SourceManager),
                                                   target->getSourceRange());
      if (field_content.find(':') != string::npos)
        return;
      if (field_content.find('=') != string::npos)
        return;
      llvm::outs() << "Mut193: " << field_content << '\n';
      if (field_content.find(' ') != string::npos)
        field_content.erase(field_content.find(' '), 1);
      content.replace(content.find(field_content), field_content.length(),
                      field_content);
    } else if (choice == 1) {
      if (methods.empty())
        return;
      int index = getrandom::getRandomIndex(std::distance(methods.begin(), methods.end()) - 1);
      auto target = *(methods.begin() + index);
      if (!target->hasBody())
        return;
      auto method_content = stringutils::rangetoStr(*(Result.SourceManager),
                                                    target->getSourceRange());
      if (method_content.find('{') == string::npos)
        return;
      if (method_content.rfind('}') == string::npos)
        return;
      method_content.erase(method_content.find('{'),
                           method_content.rfind('}') -
                               method_content.find('{') + 1);
      content.replace(content.find('{'),
                      content.rfind('}') - content.find('{') + 1,
                      method_content);
    } else if (choice == 2) {
      if (content.find("public:") != string::npos)
        content.replace(content.find("public:"), 7, "/*mut193*/");
      else if (content.find("private:") != string::npos)
        content.replace(content.find("private:"), 8, "/*mut193*/");
      else if (content.find("protected:") != string::npos)
        content.replace(content.find("protected:"), 10, "/*mut193*/");
      else
        return;
    } else if (choice == 3) {
      if (content.rfind('}') != string::npos)
        content.erase(content.rfind('}'));
    } else if (choice == 4) {
      if (!DL->isTemplated())
        return;
      auto tpl = DL->getDescribedTemplate();
      if (tpl == nullptr)
        return;
      auto tpl_content = stringutils::rangetoStr(*(Result.SourceManager),
                                                 tpl->getSourceRange());
      if (tpl_content.find('>') != string::npos)
        tpl_content.erase(tpl_content.find('>'));
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(tpl->getSourceRange()),
                          tpl_content);
      return;
    } else
      return;
    Rewrite.ReplaceText(CharSourceRange::getTokenRange(DL->getSourceRange()),
                        content);
  }
}
  
void MutatorFrontendAction_193::MutatorASTConsumer_193::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxRecordDecl().bind("Classes");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}