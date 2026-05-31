//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Dependent_Argument_To_Diagnose_If_Lambda_383
 */ 
class MutatorFrontendAction_383 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(383)

private:
    class MutatorASTConsumer_383 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_383(Rewriter &R) : TheRewriter(R) {}
        void HandleTranslationUnit(ASTContext &Context) override;
    private:
        Rewriter &TheRewriter;
    
    };
    
    class Callback : public MatchFinder::MatchCallback {
    public:
        Callback(Rewriter &Rewrite) : Rewrite(Rewrite) {}
        void run(const MatchFinder::MatchResult &Result) override;
    private:
        Rewriter &Rewrite;
        std::vector<const clang::TemplateDecl *> template_decls;
        std::vector<const clang::FunctionDecl *> template_functions;
        std::vector<const clang::CXXRecordDecl *> template_classes;
    };
};

//source file
#include "../include/Add_Dependent_Argument_To_Diagnose_If_Lambda_383.h"

// ========================================================================================================
#define MUT383_OUTPUT 1

void MutatorFrontendAction_383::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::LambdaExpr>("Lambdas")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getBeginLoc()))
        return;
      auto attrs = DL->getLambdaClass()->getAttrs();
      if (attrs.empty())
        return;
      for (auto attr : attrs) {
        if (attr->getKind() == attr::Kind::DiagnoseIf) {
          auto text = stringutils::rangetoStr(*(Result.SourceManager),
                                              attr->getRange());
          if (text.find("vals") != string::npos)
            return;
          if (text.find("false") != string::npos)
            text.replace(text.find("false"), 5, "vals");
          else if (text.find("true") != string::npos)
            text.replace(text.find("true"), 4, "vals");
          else {
            text.insert(text.find(',') + 1, " vals,");
          }
          llvm::outs() << text;
          Rewrite.ReplaceText(attr->getRange(), text);
        }
      }
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::TemplateDecl>( "Templates")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getBeginLoc()))
        return;
      template_decls.push_back(DL);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>( "TemplateFunctions")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getBeginLoc()))
        return;
      template_functions.push_back(DL);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>( "TemplateClasses")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getBeginLoc()))
        return;
      template_classes.push_back(DL);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXMethodDecl>( "Methods")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getBeginLoc()))
        return;
      if (DL->isTemplated() == false)
        return;
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
      if (content.find("vals") != string::npos)
        return;
      if (DL->isFunctionTemplateSpecialization()) {
        auto tpl = DL->getPrimaryTemplate();
        auto name = tpl->getNameAsString();
        for (size_t i = 0; i < template_decls.size(); ++i) {
          if (template_decls[i]->getNameAsString() == name) {
            llvm::outs() << template_decls[i]->getNameAsString() << '\n';
            Rewrite.ReplaceText(template_decls[i]->getEndLoc(), 0,
                                "/*mut383*/bool... vals");
          }
        }
      }
      if (DL->isTemplateInstantiation()) {
        auto tpl = DL->getPrimaryTemplate();
        auto name = tpl->getNameAsString();
        for (size_t i = 0; i < template_functions.size(); ++i) {
          if (template_functions[i]->getNameAsString() == name) {
            llvm::outs() << template_functions[i]->getNameAsString() << '\n';
            Rewrite.ReplaceText(template_functions[i]->getEndLoc(), 0,
                                "/*mut383*/bool... vals");
          }
        }
      }
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXConstructorDecl>( "Constructors")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getBeginLoc()))
        return;
      if (DL->isTemplated() == false)
        return;
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
      if (content.find("vals") != string::npos)
        return;
      if (DL->isFunctionTemplateSpecialization()) {
        auto tpl = DL->getPrimaryTemplate();
        auto name = tpl->getNameAsString();
        for (size_t i = 0; i < template_decls.size(); ++i) {
          if (template_decls[i]->getNameAsString() == name) {
            llvm::outs() << template_decls[i]->getNameAsString() << '\n';
            Rewrite.ReplaceText(template_decls[i]->getEndLoc(), 0,
                                "/*mut383*/bool... vals");
          }
        }
      }
      if (DL->isTemplateInstantiation()) {
        auto tpl = DL->getPrimaryTemplate();
        auto name = tpl->getNameAsString();
        for (size_t i = 0; i < template_functions.size(); ++i) {
          if (template_functions[i]->getNameAsString() == name) {
            llvm::outs() << template_functions[i]->getNameAsString() << '\n';
            Rewrite.ReplaceText(template_functions[i]->getEndLoc(), 0,
                                "/*mut383*/bool... vals");
          }
        }
      }
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>( "Classes")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getBeginLoc()))
        return;
      if (DL->isTemplated() == false)
        return;
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
      if (content.find("vals") != string::npos)
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isTemplated() == false)
        return;
      if (DL->isTemplateInstantiation()) {
        auto tpl = DL->getDescribedClassTemplate();
        auto name = tpl->getNameAsString();
        for (size_t i = 0; i < template_classes.size(); ++i) {
          if (template_classes[i]->getNameAsString() == name) {
            llvm::outs() << template_classes[i]->getNameAsString() << '\n';
            Rewrite.ReplaceText(template_classes[i]->getEndLoc(), 0,
                                "/*mut383*/bool... vals");
          }
        }
      }
    }
  }

void MutatorFrontendAction_383::MutatorASTConsumer_383::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = lambdaExpr().bind("Lambdas");
    auto matcher1 = clang::ast_matchers::templateDecl().bind("Templates");
    auto matcher2 = functionDecl().bind("TemplateFunctions");
    auto matcher3 = cxxRecordDecl().bind("TemplateClasses");
    auto matcher4 = cxxMethodDecl().bind("Methods");
    auto matcher5 = cxxConstructorDecl().bind("Constructors");
    auto matcher6 = cxxRecordDecl().bind("Classes");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(matcher1, &callback);
    matchFinder.addMatcher(matcher2, &callback);
    matchFinder.addMatcher(matcher3, &callback);
    matchFinder.addMatcher(matcher4, &callback);
    matchFinder.addMatcher(matcher5, &callback);
    matchFinder.addMatcher(matcher6, &callback);
    matchFinder.matchAST(Context);
}