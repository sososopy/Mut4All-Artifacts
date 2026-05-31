//header file
#pragma once
#include "Mutator_base.h"

/**
 * Replace_Method_With_Destructor_Like_Syntax_336
 */ 
class MutatorFrontendAction_336 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(336)

private:
    class MutatorASTConsumer_336 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_336(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::CXXRecordDecl *> cur_classes;
    };
};

//source file
#include "../include/replace_method_with_destructor_like_syntax_336.h"

// ========================================================================================================
#define MUT336_OUTPUT 1

void MutatorFrontendAction_336::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *MT = Result.Nodes.getNodeAs<clang::CXXMethodDecl>("Methods")) {
      if (!MT || !Result.Context->getSourceManager().isWrittenInMainFile(
                     MT->getLocation()))
        return;
      if (MT->isStatic())
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             MT->getSourceRange());
      if (content.find('~') != string::npos)
        return;
      if (content.find("operator") != string::npos)
        return;
      if (content.find("main") != string::npos)
        return;
      llvm::outs() << content << '\n';
      string replacement = "";
      if (content.find("~") != string::npos)
        return;
      if (content.find("operator") != string::npos)
        return;
      if (content.find("main") != string::npos)
        return;
      if (content.find("::") != string::npos) {
        auto pos = content.rfind("::");
        replacement = content.substr(0, pos + 2) + "~" +
                      content.substr(pos + 2, content.size() - pos - 2);
      } else {
        replacement = "~" + content;
      }
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(MT->getSourceRange()), replacement);
    } else if (auto *CL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>(
                   "Classes")) {
      if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CL->getLocation()))
        return;
      if ((!CL->isStruct() && !CL->isClass()) || CL->isLambda())
        return;
      cur_classes.push_back(CL);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXMethodDecl>(
                   "DeclMethods")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isStatic())
        return;
      if (DL->isUserProvided())
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             DL->getSourceRange());
      if (content.find('~') != string::npos)
        return;
      if (content.find("operator") != string::npos)
        return;
      if (content.find("main") != string::npos)
        return;
      llvm::outs() << content << '\n';
      string replacement = "";
      if (content.find("~") != string::npos)
        return;
      if (content.find("operator") != string::npos)
        return;
      if (content.find("main") != string::npos)
        return;
      if (content.find("::") != string::npos) {
        auto pos = content.rfind("::");
        replacement = content.substr(0, pos + 2) + "~" +
                      content.substr(pos + 2, content.size() - pos - 2);
      } else {
        replacement = "~" + content;
      }
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(DL->getSourceRange()), replacement);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXMethodDecl>(
                   "DeclMethods")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isStatic())
        return;
      if (DL->isUserProvided())
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             DL->getSourceRange());
      if (content.find('~') != string::npos)
        return;
      if (content.find("operator") != string::npos)
        return;
      if (content.find("main") != string::npos)
        return;
      llvm::outs() << content << '\n';
      string replacement = "";
      if (content.find("~") != string::npos)
        return;
      if (content.find("operator") != string::npos)
        return;
      if (content.find("main") != string::npos)
        return;
      if (content.find("::") != string::npos) {
        auto pos = content.rfind("::");
        replacement = content.substr(0, pos + 2) + "~" +
                      content.substr(pos + 2, content.size() - pos - 2);
      } else {
        replacement = "~" + content;
      }
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(DL->getSourceRange()), replacement);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXMethodDecl>(
                   "DeclMethods")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isStatic())
        return;
      if (DL->isUserProvided())
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             DL->getSourceRange());
      if (content.find('~') != string::npos)
        return;
      if (content.find("operator") != string::npos)
        return;
      if (content.find("main") != string::npos)
        return;
      llvm::outs() << content << '\n';
      string replacement = "";
      if (content.find("~") != string::npos)
        return;
      if (content.find("operator") != string::npos)
        return;
      if (content.find("main") != string::npos)
        return;
      if (content.find("::") != string::npos) {
        auto pos = content.rfind("::");
        replacement = content.substr(0, pos + 2) + "~" +
                      content.substr(pos + 2, content.size() - pos - 2);
      } else {
        replacement = "~" + content;
      }
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(DL->getSourceRange()), replacement);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXMethodDecl>(
                   "DeclMethods")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isStatic())
        return;
      if (DL->isUserProvided())
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             DL->getSourceRange());
      if (content.find('~') != string::npos)
        return;
      if (content.find("operator") != string::npos)
        return;
      if (content.find("main") != string::npos)
        return;
      llvm::outs() << content << '\n';
      string replacement = "";
      if (content.find("~") != string::npos)
        return;
      if (content.find("operator") != string::npos)
        return;
      if (content.find("main") != string::npos)
        return;
      if (content.find("::") != string::npos) {
        auto pos = content.rfind("::");
        replacement = content.substr(0, pos + 2) + "~" +
                      content.substr(pos + 2, content.size() - pos - 2);
      } else {
        replacement = "~" + content;
      }
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(DL->getSourceRange()), replacement);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXMethodDecl>(
                   "DeclMethods")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isStatic())
        return;
      if (DL->isUserProvided())
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             DL->getSourceRange());
      if (content.find('~') != string::npos)
        return;
      if (content.find("operator") != string::npos)
        return;
      if (content.find("main") != string::npos)
        return;
      llvm::outs() << content << '\n';
      string replacement = "";
      if (content.find("~") != string::npos)
        return;
      if (content.find("operator") != string::npos)
        return;
      if (content.find("main") != string::npos)
        return;
      if (content.find("::") != string::npos) {
        auto pos = content.rfind("::");
        replacement = content.substr(0, pos + 2) + "~" +
                      content.substr(pos + 2, content.size() - pos - 2);
      } else {
        replacement = "~" + content;
      }
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(DL->getSourceRange()), replacement);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXMethodDecl>(
                   "DeclMethods")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isStatic())
        return;
      if (DL->isUserProvided())
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             DL->getSourceRange());
      if (content.find('~') != string::npos)
        return;
      if (content.find("operator") != string::npos)
        return;
      if (content.find("main") != string::npos)
        return;
      llvm::outs() << content << '\n';
      string replacement = "";
      if (content.find("~") != string::npos)
        return;
      if (content.find("operator") != string::npos)
        return;
      if (content.find("main") != string::npos)
        return;
      if (content.find("::") != string::npos) {
        auto pos = content.rfind("::");
        replacement = content.substr(0, pos + 2) + "~" +
                      content.substr(pos + 2, content.size() - pos - 2);
      } else {
        replacement = "~" + content;
      }
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(DL->getSourceRange()), replacement);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXMethodDecl>(
                   "DeclMethods")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isStatic())
        return;
      if (DL->isUserProvided())
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             DL->getSourceRange());
      if (content.find('~') != string::npos)
        return;
      if (content.find("operator") != string::npos)
        return;
      if (content.find("main") != string::npos)
        return;
      llvm::outs() << content << '\n';
      string replacement = "";
      if (content.find("~") != string::npos)
        return;
      if (content.find("operator") != string::npos)
        return;
      if (content.find("main") != string::npos)
        return;
      if (content.find("::") != string::npos) {
        auto pos = content.rfind("::");
        replacement = content.substr(0, pos + 2) + "~" +
                      content.substr(pos + 2, content.size() - pos - 2);
      } else {
        replacement = "~" + content;
      }
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(DL->getSourceRange()), replacement);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXMethodDecl>(
                   "DeclMethods")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isStatic())
        return;
      if (DL->isUserProvided())
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             DL->getSourceRange());
      if (content.find('~') != string::npos)
        return;
      if (content.find("operator") != string::npos)
        return;
      if (content.find("main") != string::npos)
        return;
      llvm::outs() << content << '\n';
      string replacement = "";
      if (content.find("~") != string::npos)
        return;
      if (content.find("operator") != string::npos)
        return;
      if (content.find("main") != string::npos)
        return;
      if (content.find("::") != string::npos) {
        auto pos = content.rfind("::");
        replacement = content.substr(0, pos + 2) + "~" +
                      content.substr(pos + 2, content.size() - pos - 2);
      } else {
        replacement = "~" + content;
      }
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(DL->getSourceRange()), replacement);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXMethodDecl>(
                   "DeclMethods")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isStatic())
        return;
      if (DL->isUserProvided())
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             DL->getSourceRange());
      if (content.find('~') != string::npos)
        return;
      if (content.find("operator") != string::npos)
        return;
      if (content.find("main") != string::npos)
        return;
      llvm::outs() << content << '\n';
      string replacement = "";
      if (content.find("~") != string::npos)
        return;
      if (content.find("operator") != string::npos)
        return;
      if (content.find("main") != string::npos)
        return;
      if (content.find("::") != string::npos) {
        auto pos = content.rfind("::");
        replacement = content.substr(0, pos + 2) + "~" +
                      content.substr(pos + 2, content.size() - pos - 2);
      } else {
        replacement = "~" + content;
      }
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(DL->getSourceRange()), replacement);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXMethodDecl>(
                   "DeclMethods")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isStatic())
        return;
      if (DL->isUserProvided())
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             DL->getSourceRange());
      if (content.find('~') != string::npos)
        return;
      if (content.find("operator") != string::npos)
        return;
      if (content.find("main") != string::npos)
        return;
      llvm::outs() << content << '\n';
      string replacement = "";
      if (content.find("~") != string::npos)
        return;
      if (content.find("operator") != string::npos)
        return;
      if (content.find("main") != string::npos)
        return;
      if (content.find("::") != string::npos) {
        auto pos = content.rfind("::");
        replacement = content.substr(0, pos + 2) + "~" +
                      content.substr(pos + 2, content.size() - pos - 2);
      } else {
        replacement = "~" + content;
      }
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(DL->getSourceRange()), replacement);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXMethodDecl>(
                   "DeclMethods")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isStatic())
        return;
      if (DL->isUserProvided())
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             DL->getSourceRange());
      if (content.find('~') != string::npos)
        return;
      if (content.find("operator") != string::npos)
        return;
      if (content.find("main") != string::npos)
        return;
      llvm::outs() << content << '\n';
      string replacement = "";
      if (content.find("~") != string::npos)
        return;
      if (content.find("operator") != string::npos)
        return;
      if (content.find("main") != string::npos)
        return;
      if (content.find("::") != string::npos) {
        auto pos = content.rfind("::");
        replacement = content.substr(0, pos + 2) + "~" +
                      content.substr(pos + 2, content.size() - pos - 2);
      } else {
        replacement = "~" + content;
      }
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(DL->getSourceRange()), replacement);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXMethodDecl>(
                   "DeclMethods")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isStatic())
        return;
      if (DL->isUserProvided())
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             DL->getSourceRange());
      if (content.find('~') != string::npos)
        return;
      if (content.find("operator") != string::npos)
        return;
      if (content.find("main") != string::npos)
        return;
      llvm::outs() << content << '\n';
      string replacement = "";
      if (content.find("~") != string::npos)
        return;
      if (content.find("operator") != string::npos)
        return;
      if (content.find("main") != string::npos)
        return;
      if (content.find("::") != string::npos) {
        auto pos = content.rfind("::");
        replacement = content.substr(0, pos + 2) + "~" +
                      content.substr(pos + 2, content.size() - pos - 2);
      } else {
        replacement = "~" + content;
      }
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(DL->getSourceRange()), replacement);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXMethodDecl>(
                   "DeclMethods")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isStatic())
        return;
      if (DL->isUserProvided())
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             DL->getSourceRange());
      if (content.find('~') != string::npos)
        return;
      if (content.find("operator") != string::npos)
        return;
      if (content.find("main") != string::npos)
        return;
      llvm::outs() << content << '\n';
      string replacement = "";
      if (content.find("~") != string::npos)
        return;
      if (content.find("operator") != string::npos)
        return;
      if (content.find("main") != string::npos)
        return;
      if (content.find("::") != string::npos) {
        auto pos = content.rfind("::");
        replacement = content.substr(0, pos + 2) + "~" +
                      content.substr(pos + 2, content.size() - pos - 2);
      } else {
        replacement = "~" + content;
      }
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(DL->getSourceRange()), replacement);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXMethodDecl>(
                   "DeclMethods")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isStatic())
        return;
      if (DL->isUserProvided())
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             DL->getSourceRange());
      if (content.find('~') != string::npos)
        return;
      if (content.find("operator") != string::npos)
        return;
      if (content.find("main") != string::npos)
        return;
      llvm::outs() << content << '\n';
      string replacement = "";
      if (content.find("~") != string::npos)
        return;
      if (content.find("operator") != string::npos)
        return;
      if (content.find("main") != string::npos)
        return;
      if (content.find("::") != string::npos) {
        auto pos = content.rfind("::");
        replacement = content.substr(0, pos + 2) + "~" +
                      content.substr(pos + 2, content.size() - pos - 2);
      } else {
        replacement = "~" + content;
      }
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(DL->getSourceRange()), replacement);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXMethodDecl>(
                   "DeclMethods")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isStatic())
        return;
      if (DL->isUserProvided())
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             DL->getSourceRange());
      if (content.find('~') != string::npos)
        return;
      if (content.find("operator") != string::npos)
        return;
      if (content.find("main") != string::npos)
        return;
      llvm::outs() << content << '\n';
      string replacement = "";
      if (content.find("~") != string::npos)
        return;
      if (content.find("operator") != string::npos)
        return;
      if (content.find("main") != string::npos)
        return;
      if (content.find("::") != string::npos) {
        auto pos = content.rfind("::");
        replacement = content.substr(0, pos + 2) + "~" +
                      content.substr(pos + 2, content.size() - pos - 2);
      } else {
        replacement = "~" + content;
      }
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(DL->getSourceRange()), replacement);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXMethodDecl>(
                   "DeclMethods")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isStatic())
        return;
      if (DL->isUserProvided())
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             DL->getSourceRange());
      if (content.find('~') != string::npos)
        return;
      if (content.find("operator") != string::npos)
        return;
      if (content.find("main") != string::npos)
        return;
      llvm::outs() << content << '\n';
      string replacement = "";
      if (content.find("~") != string::npos)
        return;
      if (content.find("operator") != string::npos)
        return;
      if (content.find("main") != string::npos)
        return;
      if (content.find("::") != string::npos) {
        auto pos = content.rfind("::");
        replacement = content.substr(0, pos + 2) + "~" +
                      content.substr(pos + 2, content.size() - pos - 2);
      } else {
        replacement = "~" + content;
      }
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(DL->getSourceRange()), replacement);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXMethodDecl>(
                   "DeclMethods")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isStatic())
        return;
      if (DL->isUserProvided())
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             DL->getSourceRange());
      if (content.find('~') != string::npos)
        return;
      if (content.find("operator") != string::npos)
        return;
      if (content.find("main") != string::npos)
        return;
      llvm::outs() << content << '\n';
      string replacement = "";
      if (content.find("~") != string::npos)
        return;
      if (content.find("operator") != string::npos)
        return;
      if (content.find("main") != string::npos)
        return;
      if (content.find("::") != string::npos) {
        auto pos = content.rfind("::");
        replacement = content.substr(0, pos + 2) + "~" +
                      content.substr(pos + 2, content.size() - pos - 2);
      } else {
        replacement = "~" + content;
      }
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(DL->getSourceRange()), replacement);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXMethodDecl>(
                   "DeclMethods")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isStatic())
        return;
      if (DL->isUserProvided())
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             DL->getSourceRange());
      if (content.find('~') != string::npos)
        return;
      if (content.find("operator") != string::npos)
        return;
      if (content.find("main") != string::npos)
        return;
      llvm::outs() << content << '\n';
      string replacement = "";
      if (content.find("~") != string::npos)
        return;
      if (content.find("operator") != string::npos)
        return;
      if (content.find("main") != string::npos)
        return;
      if (content.find("::") != string::npos) {
        auto pos = content.rfind("::");
        replacement = content.substr(0, pos + 2) + "~" +
                      content.substr(pos + 2, content.size() - pos - 2);
      } else {
        replacement = "~" + content;
      }
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(DL->getSourceRange()), replacement);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXMethodDecl>(
                   "DeclMethods")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isStatic())
        return;
      if (DL->isUserProvided())
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             DL->getSourceRange());
      if (content.find('~') != string::npos)
        return;
      if (content.find("operator") != string::npos)
        return;
      if (content.find("main") != string::npos)
        return;
      llvm::outs() << content << '\n';
      string replacement = "";
      if (content.find("~") != string::npos)
        return;
      if (content.find("operator") != string::npos)
        return;
      if (content.find("main") != string::npos)
        return;
      if (content.find("::") != string::npos) {
        auto pos = content.rfind("::");
        replacement = content.substr(0, pos + 2) + "~" +
                      content.substr(pos + 2, content.size() - pos - 2);
      } else {
        replacement = "~" + content;
      }
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(DL->getSourceRange()), replacement);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXMethodDecl>(
                   "DeclMethods")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isStatic())
        return;
      if (DL->isUserProvided())
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             DL->getSourceRange());
      if (content.find('~') != string::npos)
        return;
      if (content.find("operator") != string::npos)
        return;
      if (content.find("main") != string::npos)
        return;
      llvm::outs() << content << '\n';
      string replacement = "";
      if (content.find("~") != string::npos)
        return;
      if (content.find("operator") != string::npos)
        return;
      if (content.find("main") != string::npos)
        return;
      if (content.find("::") != string::npos) {
        auto pos = content.rfind("::");
        replacement = content.substr(0, pos + 2) + "~" +
                      content.substr(pos + 2, content.size() - pos - 2);
      } else {
        replacement = "~" + content;
      }
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(DL->getSourceRange()), replacement);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXMethodDecl>(
                   "DeclMethods")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isStatic())
        return;
      if (DL->isUserProvided())
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             DL->getSourceRange());
      if (content.find('~') != string::npos)
        return;
      if (content.find("operator") != string::npos)
        return;
      if (content.find("main") != string::npos)
        return;
      llvm::outs() << content << '\n';
      string replacement = "";
      if (content.find("~") != string::npos)
        return;
      if (content.find("operator") != string::npos)
        return;
      if (content.find("main") != string::npos)
        return;
      if (content.find("::") != string::npos) {
        auto pos = content.rfind("::");
        replacement = content.substr(0, pos + 2) + "~" +
                      content.substr(pos + 2, content.size() - pos - 2);
      } else {
        replacement = "~" + content;
      }
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(DL->getSourceRange()), replacement);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXMethodDecl>(
                   "DeclMethods")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isStatic())
        return;
      if (DL->isUserProvided())
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             DL->getSourceRange());
      if (content.find('~') != string::npos)
        return;
      if (content.find("operator") != string::npos)
        return;
      if (content.find("main") != string::npos)
        return;
      llvm::outs() << content << '\n';
      string replacement = "";
      if (content.find("~") != string::npos)
        return;
      if (content.find("operator") != string::npos)
        return;
      if (content.find("main") != string::npos)
        return;
      if (content.find("::") != string::npos) {
        auto pos = content.rfind("::");
        replacement = content.substr(0, pos + 2) + "~" +
                      content.substr(pos + 2, content.size() - pos - 2);
      } else {
        replacement = "~" + content;
      }
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(DL->getSourceRange()), replacement);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXMethodDecl>(
                   "DeclMethods")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isStatic())
        return;
      if (DL->isUserProvided())
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             DL->getSourceRange());
      if (content.find('~') != string::npos)
        return;
      if (content.find("operator") != string::npos)
        return;
      if (content.find("main") != string::npos)
        return;
      llvm::outs() << content << '\n';
      string replacement = "";
      if (content.find("~") != string::npos)
        return;
      if (content.find("operator") != string::npos)
        return;
      if (content.find("main") != string::npos)
        return;
      if (content.find("::") != string::npos) {
        auto pos = content.rfind("::");
        replacement = content.substr(0, pos + 2) + "~" +
                      content.substr(pos + 2, content.size() - pos - 2);
      } else {
        replacement = "~" + content;
      }
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(DL->getSourceRange()), replacement);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXMethodDecl>(
                   "DeclMethods")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isStatic())
        return;
      if (DL->isUserProvided())
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             DL->getSourceRange());
      if (content.find('~') != string::npos)
        return;
      if (content.find("operator") != string::npos)
        return;
      if (content.find("main") != string::npos)
        return;
      llvm::outs() << content << '\n';
      string replacement = "";
      if (content.find("~") != string::npos)
        return;
      if (content.find("operator") != string::npos)
        return;
      if (content.find("main") != string::npos)
        return;
      if (content.find("::") != string::npos) {
        auto pos = content.rfind("::");
        replacement = content.substr(0, pos + 2) + "~" +
                      content.substr(pos + 2, content.size() - pos - 2);
      } else {
        replacement = "~" + content;
      }
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(DL->getSourceRange()), replacement);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXMethodDecl>(
                   "DeclMethods")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isStatic())
        return;
      if (DL->isUserProvided())
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             DL->getSourceRange());
      if (content.find('~') != string::npos)
        return;
      if (content.find("operator") != string::npos)
        return;
      if (content.find("main") != string::npos)
        return;
      llvm::outs() << content << '\n';
      string replacement = "";
      if (content.find("~") != string::npos)
        return;
      if (content.find("operator") != string::npos)
        return;
      if (content.find("main") != string::npos)
        return;
      if (content.find("::") != string::npos) {
        auto pos = content.rfind("::");
        replacement = content.substr(0, pos + 2) + "~" +
                      content.substr(pos + 2, content.size() - pos - 2);
      } else {
        replacement = "~" + content;
      }
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(DL->getSourceRange()), replacement);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXMethodDecl>(
                   "DeclMethods")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isStatic())
        return;
      if (DL->isUserProvided())
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             DL->getSourceRange());
      if (content.find('~') != string::npos)
        return;
      if (content.find("operator") != string::npos)
        return;
      if (content.find("main") != string::npos)
        return;
      llvm::outs() << content << '\n';
      string replacement = "";
      if (content.find("~") != string::npos)
        return;
      if (content.find("operator") != string::npos)
        return;
      if (content.find("main") != string::npos)
        return;
      if (content.find("::") != string::npos) {
        auto pos = content.rfind("::");
        replacement = content.substr(0, pos + 2) + "~" +
                      content.substr(pos + 2, content.size() - pos - 2);
      } else {
        replacement = "~" + content;
      }
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(DL->getSourceRange()), replacement);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXMethodDecl>(
                   "DeclMethods")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isStatic())
        return;
      if (DL->isUserProvided())
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             DL->getSourceRange());
      if (content.find('~') != string::npos)
        return;
      if (content.find("operator") != string::npos)
        return;
      if (content.find("main") != string::npos)
        return;
      llvm::outs() << content << '\n';
      string replacement = "";
      if (content.find("~") != string::npos)
        return;
      if (content.find("operator") != string::npos)
        return;
      if (content.find("main") != string::npos)
        return;
      if (content.find("::") != string::npos) {
        auto pos = content.rfind("::");
        replacement = content.substr(0, pos + 2) + "~" +
                      content.substr(pos + 2, content.size() - pos - 2);
      } else {
        replacement = "~" + content;
      }
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(DL->getSourceRange()), replacement);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXMethodDecl>(
                   "DeclMethods")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isStatic())
        return;
      if (DL->isUserProvided())
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             DL->getSourceRange());
      if (content.find('~') != string::npos)
        return;
      if (content.find("operator") != string::npos)
        return;
      if (content.find("main") != string::npos)
        return;
      llvm::outs() << content << '\n';
      string replacement = "";
      if (content.find("~") != string::npos)
        return;
      if (content.find("operator") != string::npos)
        return;
      if (content.find("main") != string::npos)
        return;
      if (content.find("::") != string::npos) {
        auto pos = content.rfind("::");
        replacement = content.substr(0, pos + 2) + "~" +
                      content.substr(pos + 2, content.size() - pos - 2);
      } else {
        replacement = "~" + content;
      }
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(DL->getSourceRange()), replacement);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXMethodDecl>(
                   "DeclMethods")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isStatic())
        return;
      if (DL->isUserProvided())
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             DL->getSourceRange());
      if (content.find('~') != string::npos)
        return;
      if (content.find("operator") != string::npos)
        return;
      if (content.find("main") != string::npos)
        return;
      llvm::outs() << content << '\n';
      string replacement = "";
      if (content.find("~") != string::npos)
        return;
      if (content.find("operator") != string::npos)
        return;
      if (content.find("main") != string::npos)
        return;
      if (content.find("::") != string::npos) {
        auto pos = content.rfind("::");
        replacement = content.substr(0, pos + 2) + "~" +
                      content.substr(pos + 2, content.size() - pos - 2);
      } else {
        replacement = "~" + content;
      }
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(DL->getSourceRange()), replacement);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXMethodDecl>(
                   "DeclMethods")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isStatic())
        return;
      if (DL->isUserProvided())
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             DL->getSourceRange());
      if (content.find('~') != string::npos)
        return;
      if (content.find("operator") != string::npos)
        return;
      if (content.find("main") != string::npos)
        return;
      llvm::outs() << content << '\n';
      string replacement = "";
      if (content.find("~") != string::npos)
        return;
      if (content.find("operator") != string::npos)
        return;
      if (content.find("main") != string::npos)
        return;
      if (content.find("::") != string::npos) {
        auto pos = content.rfind("::");
        replacement = content.substr(0, pos + 2) + "~" +
                      content.substr(pos + 2, content.size() - pos - 2);
      } else {
        replacement = "~" + content;
      }
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(DL->getSourceRange()), replacement);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXMethodDecl>(
                   "DeclMethods")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isStatic())
        return;
      if (DL->isUserProvided())
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             DL->getSourceRange());
      if (content.find('~') != string::npos)
        return;
      if (content.find("operator") != string::npos)
        return;
      if (content.find("main") != string::npos)
        return;
      llvm::outs() << content << '\n';
      string replacement = "";
      if (content.find("~") != string::npos)
        return;
      if (content.find("operator") != string::npos)
        return;
      if (content.find("main") != string::npos)
        return;
      if (content.find("::") != string::npos) {
        auto pos = content.rfind("::");
        replacement = content.substr(0, pos + 2) + "~" +
                      content.substr(pos + 2, content.size() - pos - 2);
      } else {
        replacement = "~" + content;
      }
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(DL->getSourceRange()), replacement);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXMethodDecl>(
                   "DeclMethods")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isStatic())
        return;
      if (DL->isUserProvided())
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             DL->getSourceRange());
      if (content.find('~') != string::npos)
        return;
      if (content.find("operator") != string::npos)
        return;
      if (content.find("main") != string::npos)
        return;
      llvm::outs() << content << '\n';
      string replacement = "";
      if (content.find("~") != string::npos)
        return;
      if (content.find("operator") != string::npos)
        return;
      if (content.find("main") != string::npos)
        return;
      if (content.find("::") != string::npos) {
        auto pos = content.rfind("::");
        replacement = content.substr(0, pos + 2) + "~" +
                      content.substr(pos + 2, content.size() - pos - 2);
      } else {
        replacement = "~" + content;
      }
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(DL->getSourceRange()), replacement);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXMethodDecl>(
                   "DeclMethods")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isStatic())
        return;
      if (DL->isUserProvided())
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             DL->getSourceRange());
      if (content.find('~') != string::npos)
        return;
      if (content.find("operator") != string::npos)
        return;
      if (content.find("main") != string::npos)
        return;
      llvm::outs() << content << '\n';
      string replacement = "";
      if (content.find("~") != string::npos)
        return;
      if (content.find("operator") != string::npos)
        return;
      if (content.find("main") != string::npos)
        return;
      if (content.find("::") != string::npos) {
        auto pos = content.rfind("::");
        replacement = content.substr(0, pos + 2) + "~" +
                      content.substr(pos + 2, content.size() - pos - 2);
      } else {
        replacement = "~" + content;
      }
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(DL->getSourceRange()), replacement);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXMethodDecl>(
                   "DeclMethods")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isStatic())
        return;
      if (DL->isUserProvided())
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             DL->getSourceRange());
      if (content.find('~') != string::npos)
        return;
      if (content.find("operator") != string::npos)
        return;
      if (content.find("main") != string::npos)
        return;
      llvm::outs() << content << '\n';
      string replacement = "";
      if (content.find("~") != string::npos)
        return;
      if (content.find("operator") != string::npos)
        return;
      if (content.find("main") != string::npos)
        return;
      if (content.find("::") != string::npos) {
        auto pos = content.rfind("::");
        replacement = content.substr(0, pos + 2) + "~" +
                      content.substr(pos + 2, content.size() - pos - 2);
      } else {
        replacement = "~" + content;
      }
      Rewrite.ReplaceText(
          CharSourceRange::getTokenRange(DL->getSourceRange()), replacement);
    }
}
  
void MutatorFrontendAction_336::MutatorASTConsumer_336::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = cxxMethodDecl(unless(cxxConstructorDecl())).bind("Methods");
    auto class_matcher = cxxRecordDecl().bind("Classes");
    auto decl_matcher = cxxMethodDecl().bind("DeclMethods");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(class_matcher, &callback);
    matchFinder.addMatcher(decl_matcher, &callback);
    matchFinder.matchAST(Context);
}