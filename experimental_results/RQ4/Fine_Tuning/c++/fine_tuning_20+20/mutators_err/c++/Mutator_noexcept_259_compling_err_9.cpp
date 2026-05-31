//header file
#pragma once
#include "Mutator_base.h"

/**
 * Mutator_noexcept_259
 */ 
class MutatorFrontendAction_259 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(259)

private:
    class MutatorASTConsumer_259 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_259(Rewriter &R) : TheRewriter(R) {}
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
        const clang::CXXRecordDecl *cur_class;
    };
};

//source file
#include "../include/Mutator_noexcept_259.h"

// ========================================================================================================
#define MUT259_OUTPUT 1

void MutatorFrontendAction_259::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *FN = Result.Nodes.getNodeAs<clang::FunctionDecl>("Functions")) {
      if (!FN || !Result.Context->getSourceManager().isWrittenInMainFile(
                     FN->getLocation()))
        return;

      if (!FN->hasBody())
        return;
      if (!FN->hasDynamicExceptionSpec() && !FN->hasAttr<clang::NoThrowAttr>())
        return;
      if (!FN->isNoReturn())
        return;
      auto noexcept_expr = FN->getNoReturnAttr();
      if (!noexcept_expr)
        return;
      auto expr_str = stringutils::rangetoStr(*(Result.SourceManager),
                                              noexcept_expr->getSourceRange());
      llvm::outs() << expr_str << '\n';
      if (expr_str.find("__") != string::npos ||
          expr_str.find("::") != string::npos)
        return;
      if (expr_str.find('(') != string::npos)
        return;
      if (expr_str.find(')') != string::npos)
        return;
      if (expr_str.find('[') != string::npos)
        return;
      if (expr_str.find(']') != string::npos)
        return;
      if (expr_str.find('{') != string::npos)
        return;
      if (expr_str.find('}') != string::npos)
        return;
      if (expr_str.find('<') != string::npos)
        return;
      if (expr_str.find('>') != string::npos)
        return;
      if (expr_str.find("&&") != string::npos)
        return;
      if (expr_str.find("||") != string::npos)
        return;
      if (expr_str.find("!") != string::npos)
        return;
      if (expr_str.find('+') != string::npos)
        return;
      if (expr_str.find('-') != string::npos)
        return;
      if (expr_str.find('*') != string::npos)
        return;
      if (expr_str.find('/') != string::npos)
        return;
      if (expr_str.find('%') != string::npos)
        return;
      if (expr_str.find('=') != string::npos)
        return;
      if (expr_str.find('&') != string::npos)
        return;
      if (expr_str.find('|') != string::npos)
        return;
      if (expr_str.find('^') != string::npos)
        return;
      if (expr_str.find('~') != string::npos)
        return;
      if (expr_str.find('?') != string::npos)
        return;
      if (expr_str.find(':') != string::npos)
        return;
      if (expr_str.find(',') != string::npos)
        return;
      if (expr_str.find(';') != string::npos)
        return;
      if (expr_str.find('.') != string::npos)
        return;
      if (expr_str.find('\'') != string::npos)
        return;
      if (expr_str.find('\"') != string::npos)
        return;
      if (expr_str.find('\\') != string::npos)
        return;
      if (expr_str.find('$') != string::npos)
        return;
      if (expr_str.find('#') != string::npos)
        return;
      if (expr_str.find('@') != string::npos)
        return;
      if (expr_str.find('`') != string::npos)
        return;
      if (expr_str.find('~') != string::npos)
        return;
      if (expr_str.find('!') != string::npos)
        return;
      if (expr_str.find('^') != string::npos)
        return;
      if (expr_str.find('%') != string::npos)
        return;
      if (expr_str.find('&') != string::npos)
        return;
      if (expr_str.find('*') != string::npos)
        return;
      if (expr_str.find('(') != string::npos)
        return;
      if (expr_str.find(')') != string::npos)
        return;
      if (expr_str.find('-') != string::npos)
        return;
      if (expr_str.find('+') != string::npos)
        return;
      if (expr_str.find('=') != string::npos)
        return;
      if (expr_str.find('{') != string::npos)
        return;
      if (expr_str.find('}') != string::npos)
        return;
      if (expr_str.find('[') != string::npos)
        return;
      if (expr_str.find(']') != string::npos)
        return;
      if (expr_str.find('|') != string::npos)
        return;
      if (expr_str.find('\\') != string::npos)
        return;
      if (expr_str.find(':') != string::npos)
        return;
      if (expr_str.find(';') != string::npos)
        return;
      if (expr_str.find('\"') != string::npos)
        return;
      if (expr_str.find('\'') != string::npos)
        return;
      if (expr_str.find('<') != string::npos)
        return;
      if (expr_str.find('>') != string::npos)
        return;
      if (expr_str.find(',') != string::npos)
        return;
      if (expr_str.find('.') != string::npos)
        return;
      if (expr_str.find('?') != string::npos)
        return;
      if (expr_str.find('/') != string::npos)
        return;
      if (expr_str.find(' ') != string::npos)
        return;
      if (expr_str.find('\t') != string::npos)
        return;
      if (expr_str.find('\n') != string::npos)
        return;
      if (expr_str.find('\r') != string::npos)
        return;
      if (expr_str.find('\f') != string::npos)
        return;
      if (expr_str.find('\v') != string::npos)
        return;
      if (expr_str.find('\b') != string::npos)
        return;
      if (expr_str.find('\a') != string::npos)
        return;
      if (expr_str.find('\0') != string::npos)
        return;
      if (expr_str.find('\x1b') != string::npos)
        return;
      if (expr_str.find('\x7f') != string::npos)
        return;
      if (expr_str.find('\x80') != string::npos)
        return;
      if (expr_str.find('\x81') != string::npos)
        return;
      if (expr_str.find('\x82') != string::npos)
        return;
      if (expr_str.find('\x83') != string::npos)
        return;
      if (expr_str.find('\x84') != string::npos)
        return;
      if (expr_str.find('\x85') != string::npos)
        return;
      if (expr_str.find('\x86') != string::npos)
        return;
      if (expr_str.find('\x87') != string::npos)
        return;
      if (expr_str.find('\x88') != string::npos)
        return;
      if (expr_str.find('\x89') != string::npos)
        return;
      if (expr_str.find('\x8a') != string::npos)
        return;
      if (expr_str.find('\x8b') != string::npos)
        return;
      if (expr_str.find('\x8c') != string::npos)
        return;
      if (expr_str.find('\x8d') != string::npos)
        return;
      if (expr_str.find('\x8e') != string::npos)
        return;
      if (expr_str.find('\x8f') != string::npos)
        return;
      if (expr_str.find('\x90') != string::npos)
        return;
      if (expr_str.find('\x91') != string::npos)
        return;
      if (expr_str.find('\x92') != string::npos)
        return;
      if (expr_str.find('\x93') != string::npos)
        return;
      if (expr_str.find('\x94') != string::npos)
        return;
      if (expr_str.find('\x95') != string::npos)
        return;
      if (expr_str.find('\x96') != string::npos)
        return;
      if (expr_str.find('\x97') != string::npos)
        return;
      if (expr_str.find('\x98') != string::npos)
        return;
      if (expr_str.find('\x99') != string::npos)
        return;
      if (expr_str.find('\x9a') != string::npos)
        return;
      if (expr_str.find('\x9b') != string::npos)
        return;
      if (expr_str.find('\x9c') != string::npos)
        return;
      if (expr_str.find('\x9d') != string::npos)
        return;
      if (expr_str.find('\x9e') != string::npos)
        return;
      if (expr_str.find('\x9f') != string::npos)
        return;
      if (expr_str.find('\xa0') != string::npos)
        return;
      if (expr_str.find('\xa1') != string::npos)
        return;
      if (expr_str.find('\xa2') != string::npos)
        return;
      if (expr_str.find('\xa3') != string::npos)
        return;
      if (expr_str.find('\xa4') != string::npos)
        return;
      if (expr_str.find('\xa5') != string::npos)
        return;
      if (expr_str.find('\xa6') != string::npos)
        return;
      if (expr_str.find('\xa7') != string::npos)
        return;
      if (expr_str.find('\xa8') != string::npos)
        return;
      if (expr_str.find('\xa9') != string::npos)
        return;
      if (expr_str.find('\xaa') != string::npos)
        return;
      if (expr_str.find('\xab') != string::npos)
        return;
      if (expr_str.find('\xac') != string::npos)
        return;
      if (expr_str.find('\xad') != string::npos)
        return;
      if (expr_str.find('\xae') != string::npos)
        return;
      if (expr_str.find('\xaf') != string::npos)
        return;
      if (expr_str.find('\xb0') != string::npos)
        return;
      if (expr_str.find('\xb1') != string::npos)
        return;
      if (expr_str.find('\xb2') != string::npos)
        return;
      if (expr_str.find('\xb3') != string::npos)
        return;
      if (expr_str.find('\xb4') != string::npos)
        return;
      if (expr_str.find('\xb5') != string::npos)
        return;
      if (expr_str.find('\xb6') != string::npos)
        return;
      if (expr_str.find('\xb7') != string::npos)
        return;
      if (expr_str.find('\xb8') != string::npos)
        return;
      if (expr_str.find('\xb9') != string::npos)
        return;
      if (expr_str.find('\xba') != string::npos)
        return;
      if (expr_str.find('\xbb') != string::npos)
        return;
      if (expr_str.find('\xbc') != string::npos)
        return;
      if (expr_str.find('\xbd') != string::npos)
        return;
      if (expr_str.find('\xbe') != string::npos)
        return;
      if (expr_str.find('\xbf') != string::npos)
        return;
      if (expr_str.find('\xc0') != string::npos)
        return;
      if (expr_str.find('\xc1') != string::npos)
        return;
      if (expr_str.find('\xc2') != string::npos)
        return;
      if (expr_str.find('\xc3') != string::npos)
        return;
      if (expr_str.find('\xc4') != string::npos)
        return;
      if (expr_str.find('\xc5') != string::npos)
        return;
      if (expr_str.find('\xc6') != string::npos)
        return;
      if (expr_str.find('\xc7') != string::npos)
        return;
      if (expr_str.find('\xc8') != string::npos)
        return;
      if (expr_str.find('\xc9') != string::npos)
        return;
      if (expr_str.find('\xca') != string::npos)
        return;
      if (expr_str.find('\xcb') != string::npos)
        return;
      if (expr_str.find('\xcc') != string::npos)
        return;
      if (expr_str.find('\xcd') != string::npos)
        return;
      if (expr_str.find('\xce') != string::npos)
        return;
      if (expr_str.find('\xcf') != string::npos)
        return;
      if (expr_str.find('\xd0') != string::npos)
        return;
      if (expr_str.find('\xd1') != string::npos)
        return;
      if (expr_str.find('\xd2') != string::npos)
        return;
      if (expr_str.find('\xd3') != string::npos)
        return;
      if (expr_str.find('\xd4') != string::npos)
        return;
      if (expr_str.find('\xd5') != string::npos)
        return;
      if (expr_str.find('\xd6') != string::npos)
        return;
      if (expr_str.find('\xd7') != string::npos)
        return;
      if (expr_str.find('\xd8') != string::npos)
        return;
      if (expr_str.find('\xd9') != string::npos)
        return;
      if (expr_str.find('\xda') != string::npos)
        return;
      if (expr_str.find('\xdb') != string::npos)
        return;
      if (expr_str.find('\xdc') != string::npos)
        return;
      if (expr_str.find('\xdd') != string::npos)
        return;
      if (expr_str.find('\xde') != string::npos)
        return;
      if (expr_str.find('\xdf') != string::npos)
        return;
      if (expr_str.find('\xe0') != string::npos)
        return;
      if (expr_str.find('\xe1') != string::npos)
        return;
      if (expr_str.find('\xe2') != string::npos)
        return;
      if (expr_str.find('\xe3') != string::npos)
        return;
      if (expr_str.find('\xe4') != string::npos)
        return;
      if (expr_str.find('\xe5') != string::npos)
        return;
      if (expr_str.find('\xe6') != string::npos)
        return;
      if (expr_str.find('\xe7') != string::npos)
        return;
      if (expr_str.find('\xe8') != string::npos)
        return;
      if (expr_str.find('\xe9') != string::npos)
        return;
      if (expr_str.find('\xea') != string::npos)
        return;
      if (expr_str.find('\xeb') != string::npos)
        return;
      if (expr_str.find('\xec') != string::npos)
        return;
      if (expr_str.find('\xed') != string::npos)
        return;
      if (expr_str.find('\xee') != string::npos)
        return;
      if (expr_str.find('\xef') != string::npos)
        return;
      if (expr_str.find('\xf0') != string::npos)
        return;
      if (expr_str.find('\xf1') != string::npos)
        return;
      if (expr_str.find('\xf2') != string::npos)
        return;
      if (expr_str.find('\xf3') != string::npos)
        return;
      if (expr_str.find('\xf4') != string::npos)
        return;
      if (expr_str.find('\xf5') != string::npos)
        return;
      if (expr_str.find('\xf6') != string::npos)
        return;
      if (expr_str.find('\xf7') != string::npos)
        return;
      if (expr_str.find('\xf8') != string::npos)
        return;
      if (expr_str.find('\xf9') != string::npos)
        return;
      if (expr_str.find('\xfa') != string::npos)
        return;
      if (expr_str.find('\xfb') != string::npos)
        return;
      if (expr_str.find('\xfc') != string::npos)
        return;
      if (expr_str.find('\xfd') != string::npos)
        return;
      if (expr_str.find('\xfe') != string::npos)
        return;
      if (expr_str.find('\xff') != string::npos)
        return;

      auto classes = Result.Context->getTranslationUnitDecl()->decls();
      for (auto decl : classes) {
        if (auto *CL = dyn_cast<clang::CXXRecordDecl>(decl)) {
          if (CL->isCompleteDefinition() &&
              Result.Context->getSourceManager().isWrittenInMainFile(
                  CL->getLocation())) {
            cur_class = CL;
            break;
          }
        }
      }
      if (!cur_class)
        return;
      auto classname = cur_class->getNameAsString();
      auto replacement = "std::declval<" + classname + ">().mut_259()";
      Rewrite.ReplaceText(noexcept_expr->getSourceRange(), replacement);
    } else if (auto *CL =
                   Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Classes")) {
      if (!CL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     CL->getLocation()))
        return;
      if (!CL->isCompleteDefinition())
        return;
      if (CL != cur_class)
        return;
      auto content =
          stringutils::rangetoStr(*(Result.SourceManager), CL->getSourceRange());
      auto ins = "\n\t/*mut259*/int mut_259() const {return 0;}\n";
      if (content.rfind('}') != string::npos)
        content.insert(content.rfind('}'), ins);
      Rewrite.ReplaceText(CharSourceRange::getTokenRange(CL->getSourceRange()),
                          content);
    }
}
  
void MutatorFrontendAction_259::MutatorASTConsumer_259::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    auto matcher = functionDecl().bind("Functions");
    auto class_matcher = cxxRecordDecl().bind("Classes");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(class_matcher, &callback);
    matchFinder.matchAST(Context);
}