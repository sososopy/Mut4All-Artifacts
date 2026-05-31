//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Const_Generics_20
 */ 
class MutatorFrontendAction_20 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(20)

private:
    class MutatorASTConsumer_20 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_20(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<std::string> cur_generics;
    };
};

//source file
#include "../include/add_const_generics_20.h"

// ========================================================================================================
#define MUT20_OUTPUT 1

void MutatorFrontendAction_20::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
      if (DL->isLambda())
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (content.find("template") != string::npos)
        return;
      auto name = DL->getNameAsString();
      if (name == "")
        return;
      cur_generics.push_back(name);
      auto ins = "template<typename T>\n";
      content = "/*mut20*/" + string(ins) + content;
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(DL->getSourceRange()), content);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("Funcs")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isFunctionTemplateSpecialization())
        return;
      if (DL->isOverloadedOperator())
        return;
      if (DL->getDescribedFunctionTemplate())
        return;
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
      if (content.find("template") != string::npos)
        return;
      auto name = DL->getNameAsString();
      if (name == "")
        return;
      cur_generics.push_back(name);
      auto ins = "template<typename T>\n";
      content = "/*mut20*/" + string(ins) + content;
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(DL->getSourceRange()), content);
    } else if (auto *DL =
                   Result.Nodes.getNodeAs<clang::VarDecl>("GlobalVars")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isStaticLocal())
        return;
      if (DL->isStaticDataMember())
        return;
      if (DL->isLocalVarDeclOrParm())
        return;
      if (DL->isVarTemplateSpecialization())
        return;
      if (DL->getDescribedVarTemplate())
        return;
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
      if (content.find("template") != string::npos)
        return;
      auto name = DL->getNameAsString();
      if (name == "")
        return;
      cur_generics.push_back(name);
      auto ins = "template<typename T>\n";
      content = "/*mut20*/" + string(ins) + content;
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(DL->getSourceRange()), content);
    } else if (auto *DL =
                   Result.Nodes.getNodeAs<clang::VarDecl>("LocalVars")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isStaticDataMember())
        return;
      if (DL->isVarTemplateSpecialization())
        return;
      if (DL->getDescribedVarTemplate())
        return;
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
      if (content.find("template") != string::npos)
        return;
      auto name = DL->getNameAsString();
      if (name == "")
        return;
      cur_generics.push_back(name);
      auto ins = "template<typename T>\n";
      content = "/*mut20*/" + string(ins) + content;
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(DL->getSourceRange()), content);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::EnumDecl>("Enums")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
      if (content.find("template") != string::npos)
        return;
      auto name = DL->getNameAsString();
      if (name == "")
        return;
      cur_generics.push_back(name);
      auto ins = "template<typename T>\n";
      content = "/*mut20*/" + string(ins) + content;
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(DL->getSourceRange()), content);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::TypedefDecl>("Typedefs")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
      if (content.find("template") != string::npos)
        return;
      auto name = DL->getNameAsString();
      if (name == "")
        return;
      cur_generics.push_back(name);
      auto ins = "template<typename T>\n";
      content = "/*mut20*/" + string(ins) + content;
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(DL->getSourceRange()), content);
    } else if (auto *DL =
                   Result.Nodes.getNodeAs<clang::TypeAliasDecl>("TypeAlias")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
      if (content.find("template") != string::npos)
        return;
      auto name = DL->getNameAsString();
      if (name == "")
        return;
      cur_generics.push_back(name);
      auto ins = "template<typename T>\n";
      content = "/*mut20*/" + string(ins) + content;
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(DL->getSourceRange()), content);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("CL")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      if (DL->isStruct() == false && DL->isClass() == false)
        return;
      if (DL->isLambda())
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
      if (content.find("template") == string::npos)
        return;
      auto name = DL->getNameAsString();
      if (name == "")
        return;
      auto ins = "const ";
      if (content.find("template<typename T>") != string::npos)
        content.replace(content.find("template<typename T>"), 19,
                        "template<typename T, typename U>");
      else if (content.find("template<class T>") != string::npos)
        content.replace(content.find("template<class T>"), 16,
                        "template<class T, class U>");
      if (content.find(name) != string::npos)
        content.replace(content.find(name), name.size(), name + "<T>");
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(DL->getSourceRange()), content);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>("FL")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isFunctionTemplateSpecialization() == false)
        return;
      if (DL->isOverloadedOperator())
        return;
      if (DL->getDescribedFunctionTemplate())
        return;
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
      if (content.find("template") == string::npos)
        return;
      auto name = DL->getNameAsString();
      if (name == "")
        return;
      auto ins = "const ";
      if (content.find("template<typename T>") != string::npos)
        content.replace(content.find("template<typename T>"), 19,
                        "template<typename T, typename U>");
      else if (content.find("template<class T>") != string::npos)
        content.replace(content.find("template<class T>"), 16,
                        "template<class T, class U>");
      if (content.find(name) != string::npos)
        content.replace(content.find(name), name.size(), name + "<T>");
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(DL->getSourceRange()), content);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("GVL")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isStaticLocal())
        return;
      if (DL->isStaticDataMember())
        return;
      if (DL->isLocalVarDeclOrParm())
        return;
      if (DL->isVarTemplateSpecialization() == false)
        return;
      if (DL->getDescribedVarTemplate())
        return;
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
      if (content.find("template") == string::npos)
        return;
      auto name = DL->getNameAsString();
      if (name == "")
        return;
      auto ins = "const ";
      if (content.find("template<typename T>") != string::npos)
        content.replace(content.find("template<typename T>"), 19,
                        "template<typename T, typename U>");
      else if (content.find("template<class T>") != string::npos)
        content.replace(content.find("template<class T>"), 16,
                        "template<class T, class U>");
      if (content.find(name) != string::npos)
        content.replace(content.find(name), name.size(), name + "<T>");
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(DL->getSourceRange()), content);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("LVL")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isStaticDataMember())
        return;
      if (DL->isVarTemplateSpecialization() == false)
        return;
      if (DL->getDescribedVarTemplate())
        return;
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
      if (content.find("template") == string::npos)
        return;
      auto name = DL->getNameAsString();
      if (name == "")
        return;
      auto ins = "const ";
      if (content.find("template<typename T>") != string::npos)
        content.replace(content.find("template<typename T>"), 19,
                        "template<typename T, typename U>");
      else if (content.find("template<class T>") != string::npos)
        content.replace(content.find("template<class T>"), 16,
                        "template<class T, class U>");
      if (content.find(name) != string::npos)
        content.replace(content.find(name), name.size(), name + "<T>");
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(DL->getSourceRange()), content);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::EnumDecl>("EL")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      if (DL->isCompleteDefinition() == false)
        return;
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
      if (content.find("template") == string::npos)
        return;
      auto name = DL->getNameAsString();
      if (name == "")
        return;
      auto ins = "const ";
      if (content.find("template<typename T>") != string::npos)
        content.replace(content.find("template<typename T>"), 19,
                        "template<typename T, typename U>");
      else if (content.find("template<class T>") != string::npos)
        content.replace(content.find("template<class T>"), 16,
                        "template<class T, class U>");
      if (content.find(name) != string::npos)
        content.replace(content.find(name), name.size(), name + "<T>");
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(DL->getSourceRange()), content);
    } else if (auto *DL = Result.Nodes.getNodeAs<clang::TypedefDecl>("TDL")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
      if (content.find("template") == string::npos)
        return;
      auto name = DL->getNameAsString();
      if (name == "")
        return;
      auto ins = "const ";
      if (content.find("template<typename T>") != string::npos)
        content.replace(content.find("template<typename T>"), 19,
                        "template<typename T, typename U>");
      else if (content.find("template<class T>") != string::npos)
        content.replace(content.find("template<class T>"), 16,
                        "template<class T, class U>");
      if (content.find(name) != string::npos)
        content.replace(content.find(name), name.size(), name + "<T>");
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(DL->getSourceRange()), content);
    } else if (auto *DL =
                   Result.Nodes.getNodeAs<clang::TypeAliasDecl>("TADL")) {
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), DL->getSourceRange());
      if (content.find("template") == string::npos)
        return;
      auto name = DL->getNameAsString();
      if (name == "")
        return;
      auto ins = "const ";
      if (content.find("template<typename T>") != string::npos)
        content.replace(content.find("template<typename T>"), 19,
                        "template<typename T, typename U>");
      else if (content.find("template<class T>") != string::npos)
        content.replace(content.find("template<class T>"), 16,
                        "template<class T, class U>");
      if (content.find(name) != string::npos)
        content.replace(content.find(name), name.size(), name + "<T>");
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(DL->getSourceRange()), content);
    }
  }
  
void MutatorFrontendAction_20::MutatorASTConsumer_20::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto class_matcher = cxxRecordDecl().bind("Classes");
    auto func_matcher = functionDecl().bind("Funcs");
    auto globalvar_matcher = varDecl().bind("GlobalVars");
    auto localvar_matcher = varDecl().bind("LocalVars");
    auto enum_matcher = enumDecl().bind("Enums");
    auto typedef_matcher = typedefDecl().bind("Typedefs");
    auto typealias_matcher = typeAliasDecl().bind("TypeAlias");
    auto cl_matcher = cxxRecordDecl().bind("CL");
    auto fl_matcher = functionDecl().bind("FL");
    auto gvl_matcher = varDecl().bind("GVL");
    auto lvl_matcher = varDecl().bind("LVL");
    auto el_matcher = enumDecl().bind("EL");
    auto tdl_matcher = typedefDecl().bind("TDL");
    auto tadl_matcher = typeAliasDecl().bind("TADL");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(class_matcher, &callback);
    matchFinder.addMatcher(func_matcher, &callback);
    matchFinder.addMatcher(globalvar_matcher, &callback);
    matchFinder.addMatcher(localvar_matcher, &callback);
    matchFinder.addMatcher(enum_matcher, &callback);
    matchFinder.addMatcher(typedef_matcher, &callback);
    matchFinder.addMatcher(typealias_matcher, &callback);
    matchFinder.addMatcher(cl_matcher, &callback);
    matchFinder.addMatcher(fl_matcher, &callback);
    matchFinder.addMatcher(gvl_matcher, &callback);
    matchFinder.addMatcher(lvl_matcher, &callback);
    matchFinder.addMatcher(el_matcher, &callback);
    matchFinder.addMatcher(tdl_matcher, &callback);
    matchFinder.addMatcher(tadl_matcher, &callback);
    matchFinder.matchAST(Context);
}