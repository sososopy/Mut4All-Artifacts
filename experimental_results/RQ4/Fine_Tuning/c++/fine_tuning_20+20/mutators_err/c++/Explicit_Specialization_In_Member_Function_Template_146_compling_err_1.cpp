//header file
#pragma once
#include "Mutator_base.h"

/**
 * Explicit_Specialization_In_Member_Function_Template_146
 */ 
class MutatorFrontendAction_146 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(146)

private:
    class MutatorASTConsumer_146 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_146(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::ClassTemplateDecl *> cur_class;
    };
};

//source file
#include "../include/explicit_specialization_in_member_function_template_146.h"

// ========================================================================================================
#define MUT146_OUTPUT 1

void MutatorFrontendAction_146::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *CT = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>("ClassTemplates")) {
      if (!CT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CT->getLocation()))
        return;
      cur_class.push_back(CT);
    } else if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("Methods")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      if (!MT->isFunctionTemplateSpecialization())
        return;
      if (MT->isStatic()) {
        auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                               MT->getSourceRange());
        if (content.find("this") != string::npos)
          return;
      }
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             MT->getSourceRange());
      if (content.find("return") != string::npos) {
        if (content.find('[') != string::npos &&
            content.find(']') != string::npos) {
          auto lambda = content.substr(content.find('['),
                                       content.rfind(']') - content.find('[') +
                                           1);
          llvm::outs() << lambda << '\n';
          if (lambda.find("auto") != string::npos) {
            Rewrite.ReplaceText(
                CharSourceRange::getTokenRange(MT->getSourceRange()),
                "/*mut146*/" + content);
          }
        }
      }
    } else if (auto *FD = Result.Nodes.getNodeAs<clang::FunctionDecl>(
                   "Functions")) {
      if (!FD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FD->getLocation()))
        return;
      if (FD->isFunctionTemplateSpecialization() == false)
        return;
      if (FD->isStatic()) {
        auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                               FD->getSourceRange());
        if (content.find("this") != string::npos)
          return;
      }
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             FD->getSourceRange());
      if (content.find("return") != string::npos) {
        if (content.find('[') != string::npos &&
            content.find(']') != string::npos) {
          auto lambda = content.substr(content.find('['),
                                       content.rfind(']') - content.find('[') +
                                           1);
          llvm::outs() << lambda << '\n';
          if (lambda.find("auto") != string::npos) {
            Rewrite.ReplaceText(
                CharSourceRange::getTokenRange(FD->getSourceRange()),
                "/*mut146*/" + content);
          }
        }
      }
    } else if (auto *MD = Result.Nodes.getNodeAs<clang::CXXMethodDecl>(
                   "MethodTemplates")) {
      if (!MD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MD->getLocation()))
        return;
      if (MD->isFunctionTemplateSpecialization())
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             MD->getSourceRange());
      if (content.find("return") != string::npos) {
        if (content.find('[') != string::npos &&
            content.find(']') != string::npos) {
          auto lambda = content.substr(content.find('['),
                                       content.rfind(']') - content.find('[') +
                                           1);
          llvm::outs() << lambda << '\n';
          if (lambda.find("auto") != string::npos) {
            Rewrite.ReplaceText(
                CharSourceRange::getTokenRange(MD->getSourceRange()),
                "/*mut146*/" + content);
          }
        }
      }
    } else if (auto *LT = Result.Nodes.getNodeAs<clang::ClassTemplateDecl>(
                   "LambdaTemplates")) {
      if (!LT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     LT->getLocation()))
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             LT->getSourceRange());
      if (content.find("return") != string::npos) {
        if (content.find('[') != string::npos &&
            content.find(']') != string::npos) {
          auto lambda = content.substr(content.find('['),
                                       content.rfind(']') - content.find('[') +
                                           1);
          llvm::outs() << lambda << '\n';
          if (lambda.find("auto") != string::npos) {
            Rewrite.ReplaceText(
                CharSourceRange::getTokenRange(LT->getSourceRange()),
                "/*mut146*/" + content);
          }
        }
      }
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>(
                   "DerivedClasses")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      auto decls = DL->decls();
      for (auto decl : decls) {
        if (decl->getIdentifierNamespace() ==
            Decl::IdentifierNamespace::IDNS_Using) {
          return;
        }
      }
      for (std::size_t j = 0; j < cur_class.size(); ++j) {
        if (DL->isDerivedFrom(cur_class[j]->getTemplatedDecl())) {
          auto specs = cur_class[j]->specializations();
          for (auto spec : specs) {
            auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                                   spec->getSourceRange());
            if (content.find("return") != string::npos) {
              if (content.find('[') != string::npos &&
                  content.find(']') != string::npos) {
                auto lambda = content.substr(
                    content.find('['),
                    content.rfind(']') - content.find('[') + 1);
                llvm::outs() << lambda << '\n';
                if (lambda.find("auto") != string::npos) {
                  Rewrite.ReplaceText(
                      CharSourceRange::getTokenRange(spec->getSourceRange()),
                      "/*mut146*/" + content);
                }
              }
            }
          }
        }
      }
    }
  }

void MutatorFrontendAction_146::MutatorASTConsumer_146::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = classTemplateDecl().bind("ClassTemplates");
    auto matcher2 = cxxMethodDecl().bind("Methods");
    auto matcher3 = functionDecl().bind("Functions");
    auto matcher4 = cxxMethodDecl(isTemplateInstantiation()).bind("MethodTemplates");
    auto matcher5 = classTemplateDecl(isTemplateInstantiation()).bind("LambdaTemplates");
    auto derived_matcher = cxxRecordDecl(hasAnyBase(hasType(cxxRecordDecl()))).bind("DerivedClasses");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(matcher2, &callback);
    matchFinder.addMatcher(matcher3, &callback);
    matchFinder.addMatcher(matcher4, &callback);
    matchFinder.addMatcher(matcher5, &callback);
    matchFinder.addMatcher(derived_matcher, &callback);
    matchFinder.matchAST(Context);
}