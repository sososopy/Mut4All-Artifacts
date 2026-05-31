//source file
#include "../include/forward_declare_struct_and_incomplete_destructor_53.h"

// ========================================================================================================
#define MUT53_OUTPUT 1

void MutatorFrontendAction_53::Callback::run(const MatchFinder::MatchResult &Result) {
    if (auto *SR = Result.Nodes.getNodeAs<clang::CXXRecordDecl>("Struct")) {
        if (!SR || !Result.Context->getSourceManager().isWrittenInMainFile(SR->getLocation()))
            return;
        if (!SR->isStruct() || !SR->isCompleteDefinition())
            return;

        targetStruct = SR;
        std::string structName = SR->getNameAsString();
        std::string forwardDecl = "struct " + structName + ";";
        std::string incompleteDtor = structName + "::~" + structName + "();";
        
        SourceLocation insertLoc = SR->getBeginLoc();
        Rewrite.InsertTextBefore(insertLoc, "/*mut53*/" + forwardDecl + "\n" + incompleteDtor + "\n");
    }
}

void MutatorFrontendAction_53::MutatorASTConsumer_53::HandleTranslationUnit(ASTContext &Context) {
    MatchFinder matchFinder;
    DeclarationMatcher matcher = cxxRecordDecl(isStruct()).bind("Struct");
    Callback callback(TheRewriter);
    matchFinder.addMatcher(matcher, &callback);
    matchFinder.matchAST(Context);
}