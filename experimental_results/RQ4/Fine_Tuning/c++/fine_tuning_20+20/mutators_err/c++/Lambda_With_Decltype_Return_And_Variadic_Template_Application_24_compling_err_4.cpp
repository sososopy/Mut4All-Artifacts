//header file
#pragma once
#include "Mutator_base.h"

/**
 * Lambda_With_Decltype_Return_And_Variadic_Template_Application_24
 */ 

class MutatorFrontendAction_24 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(24)
private:
    class MutatorASTConsumer_24 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_24(Rewriter &R) : TheRewriter(R) {}
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
        std::vector<const clang::VarTemplateDecl *> cur_vartemplates;
        std::vector<const clang::FunctionTemplateDecl *> cur_functemplates;
    };
};

//source file
#include "../include/lambda_with_decltype_return_and_variadic_template_application_24.h"

// ========================================================================================================
#define MUT24_OUTPUT 1

void MutatorFrontendAction_24::Callback::run(const MatchFinder::MatchResult &Result) {
  if (auto *VT = Result.Nodes.getNodeAs<clang::VarTemplateDecl>("VarTemplates")) {
    if (!VT || !Result.Context->getSourceManager().isWrittenInMainFile(
                   VT->getLocation()))
      return;
    if (VT->getTemplateParameters()->size() == 0)
      return;
    if (VT->getTemplateParameters()->getParam(0)->isTemplateParameterPack() ==
        false)
      return;
    cur_vartemplates.push_back(VT);
  } else if (auto *FT = Result.Nodes.getNodeAs<clang::FunctionTemplateDecl>(
                 "FuncTemplates")) {
    if (!FT || !Result.Context->getSourceManager().isWrittenInMainFile(
                   FT->getLocation()))
      return;
    if (FT->getTemplateParameters()->size() == 0)
      return;
    if (FT->getTemplateParameters()->getParam(0)->isTemplateParameterPack() ==
        false)
      return;
    cur_functemplates.push_back(FT);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::VarDecl>("VarDecls")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getType().getAsString().find("array") == string::npos)
      return;
    if (DL->getType().getAsString().find("int") == string::npos)
      return;
    if (DL->getType().getAsString().find("5") == string::npos)
      return;
    auto DLname = DL->getNameAsString();
    if (DLname == "")
      return;
    if (cur_vartemplates.size() == 0)
      return;
    auto target = cur_vartemplates.back();
    auto target_name = target->getNameAsString();
    if (target_name == "")
      return;
    auto DLcontent = stringutils::rangetoStr(*(Result.SourceManager),
                                             DL->getSourceRange());
    auto DLindex = DLcontent.find(DLname);
    if (DLindex == string::npos)
      return;
    DLcontent.insert(DLindex, "/*mut24*/");
    DLcontent.insert(DLindex, target_name + "<int, double, char>");
    DLcontent.insert(DLindex, "=");
    DLcontent.insert(DLindex, "decltype(");
    DLcontent.insert(DLindex, DLname);
    DLcontent.insert(DLindex, ")");

    DLcontent.insert(DLindex, " ");
    DLcontent.insert(DLindex, "auto ");
    DLcontent.insert(DLindex, "=");
    DLcontent.insert(DLindex, "[&](){return ");
    DLcontent.insert(DLindex, "(");
    DLcontent.insert(DLindex, DLname);
    DLcontent.insert(DLindex, ");");
    DLcontent.insert(DLindex, "}");
    DLcontent.insert(DLindex, "();");
    DLcontent.insert(DLindex, "\n");
    DLcontent.insert(DLindex, "/*mut24*/");
    Rewrite.ReplaceText(
        CharSourceRange::getTokenRange(DL->getSourceRange()), DLcontent);
  } else if (auto *DL = Result.Nodes.getNodeAs<clang::FunctionDecl>(
                 "FuncDecls")) {
    if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                   DL->getLocation()))
      return;
    if (DL->getNameAsString() != "foo")
      return;
    if (DL->getNumParams() == 0)
      return;
    if (DL->getParamDecl(0)->getType().getAsString() != "int")
      return;
    if (DL->getReturnType().getAsString() != "int")
      return;
    if (cur_functemplates.size() == 0)
      return;
    auto target = cur_functemplates.back();
    auto target_name = target->getNameAsString();
    if (target_name == "")
      return;
    auto DLcontent = stringutils::rangetoStr(*(Result.SourceManager),
                                             DL->getSourceRange());
    DLcontent.insert(DLcontent.find('{') + 1, "/*mut24*/");
    DLcontent.insert(DLcontent.find('{') + 1, target_name +
                                            "<int, double, char>(0, 0.0, 'a');");
    DLcontent.insert(DLcontent.find('{') + 1, "/*mut24*/");
    DLcontent.insert(DLcontent.find('{') + 1, "auto mut24=[&](int x){return x;};");
    DLcontent.insert(DLcontent.find('{') + 1, "/*mut24*/");
    DLcontent.insert(DLcontent.find('{') + 1, "return mut24(a);");
    Rewrite.ReplaceText(
        CharSourceRange::getTokenRange(DL->getSourceRange()), DLcontent);
  }
}

void MutatorFrontendAction_24::MutatorASTConsumer_24::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto vartemplate_matcher =
        varTemplateDecl(hasAnyTemplateArgument(anything())).bind("VarTemplates");
    auto functemplate_matcher =
        functionTemplateDecl(hasAnyTemplateArgument(anything()))
            .bind("FuncTemplates");
    auto vardel_matcher = varDecl().bind("VarDecls");
    auto funcdel_matcher = functionDecl().bind("FuncDecls");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(vartemplate_matcher, &callback);
    matchFinder.addMatcher(functemplate_matcher, &callback);
    matchFinder.addMatcher(vardel_matcher, &callback);
    matchFinder.addMatcher(funcdel_matcher, &callback);
    matchFinder.matchAST(Context);
}