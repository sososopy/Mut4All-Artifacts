//header file
#pragma once
#include "Mutator_base.h"

/**
 * Add_Union_Designated_Initializer_221
 */ 
class MutatorFrontendAction_221 : public MutatorFrontendAction {
public:
    MUTATOR_FRONTEND_ACTION_CREATE_ASTCONSUMER(221)

private:
    class MutatorASTConsumer_221 : public MutatorASTConsumer {
    public:
        MutatorASTConsumer_221(Rewriter &R) : TheRewriter(R) {}
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
#include "../include/Add_Union_Designated_Initializer_221.h"

// ========================================================================================================
#define MUT221_OUTPUT 1

void MutatorFrontendAction_221::Callback::run(const MatchFinder::MatchResult &Result) {
    //Check whether the matched AST node is the target node
    if (auto *DL = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Unions")) {
      //Filter nodes in header files
      if (!DL || !Result.Context->getSourceManager().isWrittenInMainFile(
                     DL->getLocation()))
        return;

      if (!DL->isUnion())
        return;
      //Get the source code text of target node
      auto members = DL->fields();
      std::vector<FieldDecl *> fields;
      for (auto member : members) {
        fields.push_back(member);
      }
      if (fields.size() <= 1)
        return;
      if (auto *DS = Result.Nodes.getNodeAs<clang::VarDecl>("UnionVars")) {
        if (!DS || !Result.Context->getSourceManager().isWrittenInMainFile(
                       DS->getLocation()))
          return;
        auto inits = DS->getInit();
        if (inits == nullptr)
          return;
        auto content = stringutils::rangetoStr(*(Result.SourceManager),
                                               inits->getSourceRange());
        if (content.find('}') == string::npos)
          return;
        std::size_t index = getrandom::getRandomIndex(fields.size() - 1);
        auto field = fields[index];
        auto name = field->getNameAsString();
        auto type = field->getType();
        auto type_str = QualType::getAsString(type.split());
        llvm::outs() << name << ' ' << type_str << '\n';
        if (type_str == "_Bool")
          content.insert(content.rfind('}'), ", ." + name + " = false");
        else if (type_str == "int")
          content.insert(content.rfind('}'), ", ." + name + " = 0");
        else if (type_str == "float")
          content.insert(content.rfind('}'), ", ." + name + " = 0.0");
        else if (type_str == "double")
          content.insert(content.rfind('}'), ", ." + name + " = 0.0");
        else if (type_str == "char")
          content.insert(content.rfind('}'), ", ." + name + " = 'a'");
        else if (type_str == "wchar_t")
          content.insert(content.rfind('}'), ", ." + name + " = L'a'");
        else if (type_str == "char16_t")
          content.insert(content.rfind('}'), ", ." + name + " = u'a'");
        else if (type_str == "char32_t")
          content.insert(content.rfind('}'), ", ." + name + " = U'a'");
        else if (type_str == "long")
          content.insert(content.rfind('}'), ", ." + name + " = 0");
        else if (type_str == "long long")
          content.insert(content.rfind('}'), ", ." + name + " = 0");
        else if (type_str == "unsigned")
          content.insert(content.rfind('}'), ", ." + name + " = 0U");
        else if (type_str == "unsigned long")
          content.insert(content.rfind('}'), ", ." + name + " = 0UL");
        else if (type_str == "unsigned long long")
          content.insert(content.rfind('}'), ", ." + name + " = 0ULL");
        else if (type_str == "short")
          content.insert(content.rfind('}'), ", ." + name + " = 0");
        else if (type_str == "unsigned short")
          content.insert(content.rfind('}'), ", ." + name + " = 0U");
        else if (type_str == "signed char")
          content.insert(content.rfind('}'), ", ." + name + " = 0");
        else if (type_str == "unsigned char")
          content.insert(content.rfind('}'), ", ." + name + " = 0U");
        else if (type_str == "wint_t")
          content.insert(content.rfind('}'), ", ." + name + " = 0");
        else if (type_str == "size_t")
          content.insert(content.rfind('}'), ", ." + name + " = 0U");
        else if (type_str == "ptrdiff_t")
          content.insert(content.rfind('}'), ", ." + name + " = 0");
        else if (type_str == "intptr_t")
          content.insert(content.rfind('}'), ", ." + name + " = 0");
        else if (type_str == "uintptr_t")
          content.insert(content.rfind('}'), ", ." + name + " = 0U");
        else if (type_str == "intmax_t")
          content.insert(content.rfind('}'), ", ." + name + " = 0");
        else if (type_str == "uintmax_t")
          content.insert(content.rfind('}'), ", ." + name + " = 0U");
        else if (type_str == "int8_t")
          content.insert(content.rfind('}'), ", ." + name + " = 0");
        else if (type_str == "uint8_t")
          content.insert(content.rfind('}'), ", ." + name + " = 0U");
        else if (type_str == "int16_t")
          content.insert(content.rfind('}'), ", ." + name + " = 0");
        else if (type_str == "uint16_t")
          content.insert(content.rfind('}'), ", ." + name + " = 0U");
        else if (type_str == "int32_t")
          content.insert(content.rfind('}'), ", ." + name + " = 0");
        else if (type_str == "uint32_t")
          content.insert(content.rfind('}'), ", ." + name + " = 0U");
        else if (type_str == "int64_t")
          content.insert(content.rfind('}'), ", ." + name + " = 0");
        else if (type_str == "uint64_t")
          content.insert(content.rfind('}'), ", ." + name + " = 0U");
        else if (type_str == "float _Complex")
          content.insert(content.rfind('}'), ", ." + name + " = 0.0f");
        else if (type_str == "double _Complex")
          content.insert(content.rfind('}'), ", ." + name + " = 0.0");
        else if (type_str == "long double _Complex")
          content.insert(content.rfind('}'), ", ." + name + " = 0.0L");
        else if (type_str == "float _Imaginary")
          content.insert(content.rfind('}'), ", ." + name + " = 0.0f");
        else if (type_str == "double _Imaginary")
          content.insert(content.rfind('}'), ", ." + name + " = 0.0");
        else if (type_str == "long double _Imaginary")
          content.insert(content.rfind('}'), ", ." + name + " = 0.0L");
        else if (type_str == "long double")
          content.insert(content.rfind('}'), ", ." + name + " = 0.0L");
        else if (type_str == "void *")
          content.insert(content.rfind('}'), ", ." + name + " = nullptr");
        else if (type_str == "const char *")
          content.insert(content.rfind('}'), ", ." + name + " = \"\"");
        else if (type_str == "std::string")
          content.insert(content.rfind('}'), ", ." + name + " = \"\"");
        else if (type_str == "std::nullptr_t")
          content.insert(content.rfind('}'), ", ." + name + " = nullptr");
        else if (type_str == "std::nullptr_t")
          content.insert(content.rfind('}'), ", ." + name + " = nullptr");
        else if (type_str == "std::nullptr_t")
          content.insert(content.rfind('}'), ", ." + name + " = nullptr");
        else
          return;
        llvm::outs() << content << '\n';
        Rewrite.ReplaceText(
            CharSourceRange::getTokenRange(inits->getSourceRange()), content);
      }
    }
}
  
void MutatorFrontendAction_221::MutatorASTConsumer_221::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    //Define one or more ASTMatchers to identify the target AST node for mutation.
    auto matcher = cxxRecordDecl().bind("Unions");
    auto var_matcher = varDecl().bind("UnionVars");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.addMatcher(var_matcher, &callback);
    matchFinder.matchAST(Context);
}