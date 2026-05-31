//source file
#include "../include/Anonymous_Structures_And_Namespaces_282.h"

// ========================================================================================================
#define MUT282_OUTPUT 1

void MutatorFrontendAction_282::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *NS = Result.Nodes.getNodeAs<clang::NamespaceDecl>("inline_namespace")) {
      if (!NS || !Result.Context->getSourceManager().isWrittenInMainFile(
                     NS->getLocation()))
        return;

      if (NS->isInlineNamespace() == false)
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             NS->getSourceRange());
      llvm::outs() << content;
      auto pos = content.find("inline namespace ");
      if (pos != string::npos) {
        pos += 16;
        while (content[pos] != ' ')
          content[pos++] = ' ';
      }
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(NS->getSourceRange()),
                          content);
    } else if (auto *ED =
                   Result.Nodes.getNodeAs<clang::EnumDecl>("named_enum")) {
      if (!ED || !Result.Context->getSourceManager().isWrittenInMainFile(
                     ED->getLocation()))
        return;

      if (ED->getNameAsString() == "")
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             ED->getSourceRange());
      llvm::outs() << content;
      auto pos = content.find("enum ");
      if (pos != string::npos) {
        pos += 5;
        while (content[pos] != '{')
          content[pos++] = ' ';
      }
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(ED->getSourceRange()),
                          content);
    } else if (auto *CD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>(
                   "named_class")) {
      if (!CD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CD->getLocation()))
        return;

      if (CD->getNameAsString() == "")
        return;
      if (CD->isStruct() == true)
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             CD->getSourceRange());
      llvm::outs() << content;
      auto pos = content.find("class ");
      if (pos != string::npos) {
        pos += 6;
        while (content[pos] != '{')
          content[pos++] = ' ';
      }
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(CD->getSourceRange()),
                          content);
    } else if (auto *SD = Result.Nodes.getNodeAs<clang::CXXRecordDecl>(
                   "named_struct")) {
      if (!SD || !Result.Context->getSourceManager().isWrittenInMainFile(
                     SD->getLocation()))
        return;

      if (SD->getNameAsString() == "")
        return;
      if (SD->isStruct() == false)
        return;
      auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                             SD->getSourceRange());
      llvm::outs() << content;
      auto pos = content.find("struct ");
      if (pos != string::npos) {
        pos += 7;
        while (content[pos] != '{')
          content[pos++] = ' ';
      }
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(SD->getSourceRange()),
                          content);
    }
}
  
void MutatorFrontendAction_282::MutatorASTConsumer_282::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto inline_namespace_matcher =
        namespaceDecl().bind("inline_namespace");
    auto named_enum_matcher = enumDecl().bind("named_enum");
    auto named_class_matcher = cxxRecordDecl().bind("named_class");
    auto named_struct_matcher = cxxRecordDecl().bind("named_struct");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(inline_namespace_matcher, &callback);
    matchFinder.addMatcher(named_enum_matcher, &callback);
    matchFinder.addMatcher(named_class_matcher, &callback);
    matchFinder.addMatcher(named_struct_matcher, &callback);
    matchFinder.matchAST(Context);
}